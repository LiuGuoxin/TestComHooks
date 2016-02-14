using System;
using System.Runtime.InteropServices;

namespace CsCom
{
	[ComVisible(true)]
	[Guid("79E1CA1B-C8BE-4944-98CB-987D359B2F2A")]
	[ComDefaultInterface(typeof(CppCom.IFoo))]
	[ClassInterface(ClassInterfaceType.None)]
	public class FooHook : CppCom.IFoo, IDisposable
	{
		private bool disposed;

		public FooHook()
		{
			Console.WriteLine("C# FooHook(ctor)");
			InstallHooks();
		}

		~FooHook()
		{
			Console.WriteLine("C# ~FooHook()");
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
		}

		public void Bar()
		{
			Console.WriteLine("C# FooHook.Bar()");
		}

		private delegate int ReleaseDelegate(IntPtr unknown);
		private int ReleaseDetour(IntPtr unknown)
		{
			var result = release(unknown);
			Console.WriteLine($"C# ReleaseDetour(new count={result})");
			if (result == 0)
				Dispose();
			return result;
		}

		private ReleaseDelegate release;
		private ReleaseDelegate releaseDetour;	//Must hang on to delegate to prevent GC

		private void InstallHooks()
		{
			releaseDetour = ReleaseDetour;
			var nativeReleaseDetour = Marshal.GetFunctionPointerForDelegate(releaseDetour);
			InstallHook(Marshal.GetIUnknownForObject(this), nativeReleaseDetour);

			InstallHook(Marshal.GetComInterfaceForObject(this, typeof(CppCom.IFoo)), nativeReleaseDetour);
			InstallHook(Marshal.GetComInterfaceForObject(this, typeof(IDisposable)), nativeReleaseDetour);

			//TODO: GetComInterfaceForObject does not work for typeof(object), so what about the _Object interface?
			//Would need to do this for IDispatch (via GetIDispatchForObject) if it supported it
			//Would need to do this for all supported interfaces in case that interface is the last reference
			//TODO: What about other special interfaces automatically supported like:
			//	ITypeInfo, IObjectSafety, ISupportErrorInfo, IMarshal, IReflect, IDispatchEx, IEnumVARIANT, etc.?
		}

		private void InstallHook(IntPtr comInterface, IntPtr nativeReleaseDetour)
		{
			var vtable = Marshal.ReadIntPtr(comInterface);
			var nativeRelease = Marshal.ReadIntPtr(vtable, 2 * IntPtr.Size);
			Marshal.WriteIntPtr(vtable, 2 * IntPtr.Size, nativeReleaseDetour);
			if (release == null)
				release = Marshal.GetDelegateForFunctionPointer<ReleaseDelegate>(nativeRelease);
			release(comInterface);
		}
	}
}
