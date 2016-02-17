using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace CsCom
{
	[ComVisible(true)]
	[Guid("AAC2D72E-EE09-47F2-8A0D-836C256F9CB0")]
	[ComDefaultInterface(typeof(CppCom.IFoo))]
	[ClassInterface(ClassInterfaceType.None)]
	public class FooHook2 : CppCom.IFoo, IDisposable
	{
		private bool disposed;

		public FooHook2()
		{
			Console.WriteLine("C# FooHook2(ctor)");
			InstallHooks();
		}

		~FooHook2()
		{
			Console.WriteLine("C# ~FooHook2()");
			Dispose(false);
		}

		public void Dispose()
		{
			Dispose(true);
			GC.SuppressFinalize(this);
		}

		protected virtual void Dispose(bool releaseManagedReferences)
		{
			Console.WriteLine($"C# Dispose({releaseManagedReferences})");
			if (disposed)
				return;
			disposed = true;
			if (releaseManagedReferences)
				RemoveHooks();
			//TODO: other disposal taking into account releaseManagedReferences
		}

		public void Bar()
		{
			Console.WriteLine("C# FooHook2.Bar()");
		}

		private delegate int QueryInterfaceDelegate(IntPtr unknown, ref Guid riid, out IntPtr ppvObject);
		private delegate int ReleaseDelegate(IntPtr unknown);

		private static Guid supportErrorInfoId = new Guid("df0b3d60-548f-101b-8e65-08002b2bd119");

		private IntPtr supportErrorInfoVTable;
		private IntPtr supportErrorInfoProxy;

		private readonly List<InterfaceHooks> hooks = new List<InterfaceHooks>();

		private void InstallHooks()
		{
			Console.WriteLine("C# Installing Hooks");
			var unknown = Marshal.GetIUnknownForObject(this);
			var disposable = Marshal.GetComInterfaceForObject(this, typeof(IDisposable));
			var foo = Marshal.GetComInterfaceForObject(this, typeof(CppCom.IFoo));

			IntPtr supportErrorInfo;
			var supportErrorInfoResult = Marshal.QueryInterface(unknown, ref supportErrorInfoId, out supportErrorInfo);
			if (supportErrorInfoResult != 0)
				throw new InvalidOperationException("QueryInterface(ISupportErrorInfo) failed.");

			var vtable = Marshal.ReadIntPtr(supportErrorInfo);
			supportErrorInfoVTable = SupportErrorInfoVTable.Create(
				Marshal.ReadIntPtr(vtable, 0 * IntPtr.Size),
				Marshal.ReadIntPtr(vtable, 1 * IntPtr.Size),
				Marshal.ReadIntPtr(vtable, 2 * IntPtr.Size),
				Marshal.ReadIntPtr(vtable, 3 * IntPtr.Size));
			supportErrorInfoProxy = ComInterface.Create(supportErrorInfoVTable);
			hooks.Add(new InterfaceHooks(this, "ISupportErrorInfo", supportErrorInfoProxy, supportErrorInfoProxy, supportErrorInfo));

			hooks.Add(new InterfaceHooks(this, "IUnknown", unknown, supportErrorInfoProxy, supportErrorInfo));
			hooks.Add(new InterfaceHooks(this, "IDisposable", disposable, supportErrorInfoProxy, supportErrorInfo));
			hooks.Add(new InterfaceHooks(this, "IFoo", foo, supportErrorInfoProxy, supportErrorInfo));

			Console.WriteLine("C# Releasing interfaces...");
			Marshal.Release(unknown);
			Marshal.Release(disposable);
			Marshal.Release(foo);
			Marshal.Release(supportErrorInfoProxy);
			Console.WriteLine("C# Done hooking...");
		}

		private void RemoveHooks()
		{
			Marshal.FreeHGlobal(supportErrorInfoVTable);
			Marshal.FreeHGlobal(supportErrorInfoProxy);
			foreach (var hook in hooks)
				hook.ReleaseMarshalledDelegates();
		}

		[StructLayout(LayoutKind.Sequential)]
		private struct ComInterface
		{
			public IntPtr VTable;

			public static IntPtr Create(IntPtr vtable)
			{
				var instance = new ComInterface { VTable = vtable };
				var marshalledInstance = Marshal.AllocHGlobal(Marshal.SizeOf(instance));
				Marshal.StructureToPtr(instance, marshalledInstance, false);
				return marshalledInstance;
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		private struct SupportErrorInfoVTable
		{
			public IntPtr QueryInterface;
			public IntPtr AddRef;
			public IntPtr Release;
			public IntPtr InterfaceSupportsErrorInfo;

			public static IntPtr Create(
				IntPtr queryInterface,
				IntPtr addRef,
				IntPtr release,
				IntPtr interfaceSupportsErrorInfo)
			{
				var instance = new SupportErrorInfoVTable
				{
					QueryInterface = queryInterface,
					AddRef = addRef,
					Release = release,
					InterfaceSupportsErrorInfo = interfaceSupportsErrorInfo
				};
				var marshalledInstance = Marshal.AllocHGlobal(Marshal.SizeOf(instance));
				Marshal.StructureToPtr(instance, marshalledInstance, false);
				return marshalledInstance;
			}
		}

		private class InterfaceHooks
		{
			private ReleaseDelegate releaseDetour;
			private readonly ReleaseDelegate originalRelease;

			private QueryInterfaceDelegate queryInterfaceDetour;
			private readonly QueryInterfaceDelegate originalQueryInterface;

			private readonly IntPtr supportErrorInfoProxy;
			private readonly IntPtr supportErrorInfo;

			private readonly IDisposable comObject;
			private readonly string name;

			public InterfaceHooks(IDisposable comObject, string name, IntPtr comInterface, IntPtr supportErrorInfoProxy, IntPtr supportErrorInfo)
			{
				this.name = name;
				this.comObject = comObject;
				this.supportErrorInfoProxy = supportErrorInfoProxy;
				this.supportErrorInfo = supportErrorInfo;

				queryInterfaceDetour = QueryInterfaceDetour;
				releaseDetour = ReleaseDetour;

				var vtable = Marshal.ReadIntPtr(comInterface);
				var originalQueryInterfacePointer = Marshal.ReadIntPtr(vtable, 0 * IntPtr.Size);
				var originalReleasePointer = Marshal.ReadIntPtr(vtable, 2 * IntPtr.Size);
				originalQueryInterface = Marshal.GetDelegateForFunctionPointer<QueryInterfaceDelegate>(originalQueryInterfacePointer);
				originalRelease = Marshal.GetDelegateForFunctionPointer<ReleaseDelegate>(originalReleasePointer);
				Marshal.WriteIntPtr(vtable, 0 * IntPtr.Size, Marshal.GetFunctionPointerForDelegate(queryInterfaceDetour));
				Marshal.WriteIntPtr(vtable, 2 * IntPtr.Size, Marshal.GetFunctionPointerForDelegate(releaseDetour));
			}

			private IntPtr GetUnknown(IntPtr unknown)
			{
				return unknown == supportErrorInfoProxy ? supportErrorInfo : unknown;
			}

			private int QueryInterfaceDetour(IntPtr unknown, ref Guid riid, out IntPtr ppvObject)
			{
				Console.WriteLine($"C# QueryInterfaceDetour({riid}) via {name}");
				if (riid != supportErrorInfoId)
					return originalQueryInterface(GetUnknown(unknown), ref riid, out ppvObject);
				Marshal.AddRef(unknown);
				ppvObject = supportErrorInfoProxy;
				return 0;
			}

			private int ReleaseDetour(IntPtr unknown)
			{
				Console.WriteLine($"C# ReleaseDetour() via {name}");
				var result = originalRelease(GetUnknown(unknown));
				Console.WriteLine($"new refcount = {result}"); 
				if (result == 0)
					comObject.Dispose();
				return result;
			}

			public void ReleaseMarshalledDelegates()
			{
				releaseDetour = null;
				queryInterfaceDetour = null;
			}
		}
	}
}
