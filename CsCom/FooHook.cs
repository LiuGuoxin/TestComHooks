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
		private readonly ReleaseHooker releaseHooker;

		public FooHook()
		{
			Console.WriteLine("C# FooHook(ctor)");
			releaseHooker = new ReleaseHooker(this, typeof(CppCom.IFoo));
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
			if (releaseManagedReferences)
				releaseHooker.ReleaseMarshalledDelegate();
			//TODO: other disposal taking into account releaseManagedReferences
		}

		public void Bar()
		{
			Console.WriteLine("C# FooHook.Bar()");
		}
	}
}
