using System;
using System.Runtime.InteropServices;

namespace CsCom
{
	[ComVisible(true)]
	[Guid("AAC2D72E-EE09-47F2-8A0D-836C256F9CB0")]
	[ComDefaultInterface(typeof(CppCom.IFoo))]
	[ClassInterface(ClassInterfaceType.None)]
	public class FooHook2 : DisposableComObject, CppCom.IFoo
	{
		private bool disposed;

		public FooHook2()
		{
			Console.WriteLine("C# FooHook2(ctor)");
		}

		~FooHook2()
		{
			Console.WriteLine("C# ~FooHook2()");
		}

		protected override void Dispose(bool releaseManagedReferences)
		{
			Console.WriteLine($"C# Dispose({releaseManagedReferences})");
			base.Dispose(releaseManagedReferences);
			if (disposed)
				return;
			disposed = true;
			//TODO: other disposal taking into account releaseManagedReferences
		}

		public void Bar()
		{
			Console.WriteLine("C# FooHook2.Bar()");
		}
	}
}
