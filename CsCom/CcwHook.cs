using System;
using System.Runtime.InteropServices;

namespace CsCom
{
	internal class CcwHook
	{
		private delegate int QueryInterfaceDelegate(IntPtr unknown, ref Guid riid, out IntPtr ppvObject);
		private delegate int ReleaseDelegate(IntPtr unknown);

		private ReleaseDelegate releaseDetour;
		private readonly ReleaseDelegate originalRelease;

		private QueryInterfaceDelegate queryInterfaceDetour;
		private readonly QueryInterfaceDelegate originalQueryInterface;

		private readonly IntPtr originalInterface;
		private readonly CcwWrappers wrappers;
		private readonly IDisposable comObject;

		public CcwHook(IDisposable comObject, CcwWrapper wrapper, CcwWrappers wrappers)
			: this(comObject, wrapper.Wrapper, wrapper.Original, wrappers)
		{
		}

		public CcwHook(IDisposable comObject, IntPtr comInterface, CcwWrappers wrappers)
			: this(comObject, comInterface, comInterface, wrappers)
		{
		}

		private CcwHook(IDisposable comObject, IntPtr comInterface, IntPtr originalInterface, CcwWrappers wrappers)
		{
			this.comObject = comObject;
			this.originalInterface = originalInterface;
			this.wrappers = wrappers;

			queryInterfaceDetour = QueryInterfaceDetour;
			releaseDetour = ReleaseDetour;

			var vtable = Marshal.ReadIntPtr(comInterface);
			var originalQueryInterfacePointer = Marshal.ReadIntPtr(vtable, 0 * IntPtr.Size);
			var originalReleasePointer = Marshal.ReadIntPtr(vtable, 2 * IntPtr.Size);
			originalQueryInterface = Marshal.GetDelegateForFunctionPointer<QueryInterfaceDelegate>(originalQueryInterfacePointer);
			originalRelease = Marshal.GetDelegateForFunctionPointer<ReleaseDelegate>(originalReleasePointer);
			Marshal.WriteIntPtr(vtable, 0 * IntPtr.Size, Marshal.GetFunctionPointerForDelegate(queryInterfaceDetour));
			Marshal.WriteIntPtr(vtable, 2 * IntPtr.Size, Marshal.GetFunctionPointerForDelegate(releaseDetour));
		}

		private int QueryInterfaceDetour(IntPtr unknown, ref Guid riid, out IntPtr ppvObject)
		{
			return wrappers.TryQueryInterface(riid, out ppvObject) ?
				0 :
				originalQueryInterface(originalInterface, ref riid, out ppvObject);
		}

		private int ReleaseDetour(IntPtr unknown)
		{
			var result = originalRelease(originalInterface);
			Console.WriteLine($"C# ReleaseDetour(new refcount = {result})");
			if (result == 0)
				comObject.Dispose();
			return result;
		}

		public void ReleaseMarshalledDelegates()
		{
			releaseDetour = null;
			queryInterfaceDetour = null;
		}
	}
}