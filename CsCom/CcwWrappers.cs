using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace CsCom
{
	internal class CcwWrappers : IDisposable
	{
		private bool disposed;
		private readonly Dictionary<Guid, CcwWrapper> wrappers = new Dictionary<Guid, CcwWrapper>();

		~CcwWrappers()
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
			if (!releaseManagedReferences)
				return;
			foreach (var wrapper in wrappers.Values)
				wrapper.Dispose();
		}

		public CcwWrapper Wrap(IntPtr unknown, Guid iid, int functionCount)
		{
			IntPtr comInterface;
			var result = Marshal.QueryInterface(unknown, ref iid, out comInterface);
			if (result != 0)
				throw new COMException("QueryInterface failed.", result);
			var wrapper = new CcwWrapper(comInterface, functionCount);
			wrappers.Add(iid, wrapper);
			return wrapper;
		}

		public bool TryQueryInterface(Guid iid, out IntPtr ppvObject)
		{
			CcwWrapper wrapper;
			if (!wrappers.TryGetValue(iid, out wrapper))
			{
				ppvObject = IntPtr.Zero;
				return false;
			}
			Marshal.AddRef(wrapper.Original);
			ppvObject = wrapper.Wrapper;
			return true;
		}
	}
}