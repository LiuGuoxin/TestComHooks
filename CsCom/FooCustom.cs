using System;
using System.Runtime.InteropServices;
using System.Threading;

namespace CsCom
{
	[ComVisible(true)]
	[Guid("259B006A-93C1-408B-989C-204E5C80D2DD")]
	[ComDefaultInterface(typeof(CppCom.IFoo))]
	[ClassInterface(ClassInterfaceType.None)]
	public class FooCustom : CppCom.IFoo, IDisposable, ICustomQueryInterface
	{
		private bool disposed;
		private int referenceCounter;

		public FooCustom()
		{
			Console.WriteLine("C# FooCustom(ctor)");
		}

		~FooCustom()
		{
			Console.WriteLine("C# ~FooCustom()");
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
			//TODO: other disposal taking into account releaseManagedReferences
			if (marshalledInterface == IntPtr.Zero)
				return;
			Marshal.FreeHGlobal(marshalledInterface);
			Marshal.FreeHGlobal(marshalledVTable);
			marshalledVTable = IntPtr.Zero;
			marshalledInterface = IntPtr.Zero;
		}

		public void Bar()
		{
			Console.WriteLine("C# FooCustom.Bar()");
		}

		private delegate int QueryInterfaceDelegate(IntPtr unknown, ref Guid iid, out IntPtr ppv);
		private int QueryInterfaceDetour(IntPtr unknown, ref Guid iid, out IntPtr ppv)
		{
			Console.WriteLine($"C# QueryInterfaceDetour({iid})");

			if (iid != typeof(CppCom.IFoo).GUID)
			{
				ppv = IntPtr.Zero;
				return 1;
			}

			Interlocked.Increment(ref referenceCounter);
			ppv = marshalledInterface;
			return 0;
		}

		private delegate int AddRefDelegate(IntPtr unknown);
		private int AddRefDetour(IntPtr unknown)
		{
			Console.WriteLine("C# AddRefDetour");
			return Interlocked.Increment(ref referenceCounter);
		}

		private delegate int ReleaseDelegate(IntPtr unknown);
		private int ReleaseDetour(IntPtr unknown)
		{
			var result = Interlocked.Decrement(ref referenceCounter);
			Console.WriteLine($"C# ReleaseDetour(new count={referenceCounter})");
			if (result == 0)
				Dispose();
			return result;
		}

		private delegate void BarDelegate(IntPtr unknown);
		private void BarDetour(IntPtr unknown)
		{
			Bar();
		}

		[StructLayout(LayoutKind.Sequential)]
		private struct ManualInterface
		{
			public IntPtr VTable;

			public static IntPtr Create(IntPtr vtable)
			{
				var instance = new ManualInterface { VTable = vtable };
				var marshalledInstance = Marshal.AllocHGlobal(Marshal.SizeOf(instance));
				Marshal.StructureToPtr(instance, marshalledInstance, false);
				return marshalledInstance;
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		private struct ManualVTable
		{
			public IntPtr QueryInterface;
			public IntPtr AddRef;
			public IntPtr Release;
			public IntPtr Bar;

			public static IntPtr Create(
				QueryInterfaceDelegate queryInterface,
				AddRefDelegate addRef,
				ReleaseDelegate release,
				BarDelegate bar)
			{
				var instance = new ManualVTable
				{
					QueryInterface = Marshal.GetFunctionPointerForDelegate(queryInterface),
					AddRef = Marshal.GetFunctionPointerForDelegate(addRef),
					Release = Marshal.GetFunctionPointerForDelegate(release),
					Bar = Marshal.GetFunctionPointerForDelegate(bar)
				};
				var marshalledInstance = Marshal.AllocHGlobal(Marshal.SizeOf(instance));
				Marshal.StructureToPtr(instance, marshalledInstance, false);
				return marshalledInstance;
			}
		}

		private IntPtr marshalledVTable;
		private IntPtr marshalledInterface;

		public CustomQueryInterfaceResult GetInterface(ref Guid iid, out IntPtr ppv)
		{
			Console.WriteLine($"C# GetInterface({iid})");

			if (iid != typeof(CppCom.IFoo).GUID)
			{
				ppv = IntPtr.Zero;
				return CustomQueryInterfaceResult.NotHandled;
			}

			Interlocked.Increment(ref referenceCounter);
			if (marshalledInterface == IntPtr.Zero)
			{
				marshalledVTable = ManualVTable.Create(
					QueryInterfaceDetour,
					AddRefDetour,
					ReleaseDetour,
					BarDetour);
				marshalledInterface = ManualInterface.Create(marshalledVTable);
			}
			ppv = marshalledInterface;
			return CustomQueryInterfaceResult.Handled;
		}
	}
}
