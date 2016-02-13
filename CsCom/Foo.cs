using System;
using System.Runtime.InteropServices;

namespace CsCom
{
	[ComVisible(true)]
	[Guid("D0AD54BA-7405-49EE-B380-49D6E0D51F1C")]
	[ComDefaultInterface(typeof(CppCom.IFoo))]
	[ClassInterface(ClassInterfaceType.None)]
	public class Foo : CppCom.IFoo, IDisposable
	{
		private bool disposed;

		public Foo()
		{
			Console.WriteLine("C# Foo(ctor)");
		}

		~Foo()
		{
			Dispose(false);
			Console.WriteLine("C# ~Foo()");
		}

		public void Bar()
		{
			Console.WriteLine("C# Foo.Bar");
		}

		public void Dispose()
		{
			Dispose(true);
			GC.SuppressFinalize(this);
		}

		protected virtual void Dispose(bool releaseManagedReferences)
		{
			if (disposed)
				return;
			disposed = true;
			Console.WriteLine($"C# Dispose({releaseManagedReferences})");
		}
	}
}
