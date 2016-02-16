using System;
using System.Runtime.InteropServices;

namespace CsCom
{
	public class ReleaseHook
	{
		private delegate int ReleaseDelegate(IntPtr unknown);
		private readonly ReleaseDelegate originalRelease;
		private ReleaseDelegate releaseDetour;
		private readonly IDisposable comObject;

		public ReleaseHook(IDisposable comObject, IntPtr comInterface)
		{
			this.comObject = comObject;
			var vtable = Marshal.ReadIntPtr(comInterface);
			var releaseOffset = 2 * IntPtr.Size;
			var releasePointer = Marshal.ReadIntPtr(vtable, releaseOffset);
			originalRelease = Marshal.GetDelegateForFunctionPointer<ReleaseDelegate>(releasePointer);
			releaseDetour = ReleaseDetour;
			var releaseDetourPointer = Marshal.GetFunctionPointerForDelegate(releaseDetour);
			Marshal.WriteIntPtr(vtable, releaseOffset, releaseDetourPointer);
		}

		private int ReleaseDetour(IntPtr unknown)
		{
			var result = originalRelease(unknown);
			Console.WriteLine($"C# ReleaseDetour(new refcount={result})");
			if (result == 0)
				comObject.Dispose();
			return result;
		}

		public void ReleaseMarshalledDelegate()
		{
			releaseDetour = null;
		}
	}
}
