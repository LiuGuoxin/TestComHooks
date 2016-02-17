using System;
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
		private readonly DeterministicFinalizer deterministicFinalizer;

		public FooHook2()
		{
			Console.WriteLine("C# FooHook2(ctor)");
			deterministicFinalizer = new DeterministicFinalizer(this, typeof(CppCom.IFoo));
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
				deterministicFinalizer.Dispose();
			//TODO: other disposal taking into account releaseManagedReferences
		}

		public void Bar()
		{
			Console.WriteLine("C# FooHook2.Bar()");
		}
	}
}
