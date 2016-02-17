using System;
using System.Linq;
using System.Runtime.InteropServices;

namespace CsCom
{
	internal class CcwWrapper : IDisposable
	{
		private bool disposed;
		private readonly IntPtr vtable;

		public CcwWrapper(IntPtr original, int functionCount)
		{
			Original = original;
			var originalVTable = Marshal.ReadIntPtr(original);
			vtable = Marshal.AllocHGlobal(functionCount * IntPtr.Size);
			foreach (var offset in Enumerable.Range(0, functionCount).Select(index => index * IntPtr.Size))
				Marshal.WriteIntPtr(vtable, offset, Marshal.ReadIntPtr(originalVTable, offset));
			Wrapper = Marshal.AllocHGlobal(IntPtr.Size);
			Marshal.WriteIntPtr(Wrapper, vtable);
		}

		~CcwWrapper()
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
			Marshal.FreeHGlobal(vtable);
			Marshal.FreeHGlobal(Wrapper);
		}

		public IntPtr Original { get; }
		public IntPtr Wrapper { get; }
	}
}