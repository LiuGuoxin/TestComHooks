using System;

namespace CsCom
{
	public abstract class DisposableComObject : MarshalByRefObject, IDisposable
	{
		private bool disposed;
		private readonly DeterministicFinalizer deterministicFinalizer;

		protected DisposableComObject()
		{
			deterministicFinalizer = new DeterministicFinalizer(this);
		}

		~DisposableComObject()
		{
			Dispose(false);
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
			if (releaseManagedReferences)
				deterministicFinalizer.Dispose();
		}
	}
}
