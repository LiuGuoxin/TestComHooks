using System;
using System.Runtime.InteropServices;

namespace CsCom
{
	[ComVisible(true)]
	[ComDefaultInterface(typeof(CppCom.IFoo))]
	[ClassInterface(ClassInterfaceType.None)]
	public class FooAggregate : CppCom.IFoo, IDisposable
	{
		private bool disposed;

		public FooAggregate()
		{
			Console.WriteLine("C# FooAggregate(ctor)");
		}

		~FooAggregate()
		{
			Dispose(false);
			Console.WriteLine("C# ~FooAggregate()");
		}

		public void Bar()
		{
			Console.WriteLine("C# FooAggregate.Bar");
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
