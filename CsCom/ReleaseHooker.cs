using System;
using System.Runtime.InteropServices;

namespace CsCom
{
	public class ReleaseHooker
	{
		private delegate int ReleaseDelegate(IntPtr unknown);
		private ReleaseDelegate originalRelease;
		private ReleaseDelegate releaseDetour;
		private readonly IDisposable comObject;

		public ReleaseHooker(IDisposable comObject, params Type[] interfaces)
		{
			this.comObject = comObject;
			releaseDetour = ReleaseDetour;

			var releaseDetourPointer = Marshal.GetFunctionPointerForDelegate(releaseDetour);
			InstallHook(Marshal.GetIUnknownForObject(comObject), releaseDetourPointer);
			InstallHook(Marshal.GetComInterfaceForObject(comObject, typeof(IDisposable)), releaseDetourPointer);
			foreach (var @interface in interfaces)
				InstallHook(Marshal.GetComInterfaceForObject(comObject, @interface), releaseDetourPointer);

			//TODO: GetComInterfaceForObject does not work for typeof(object), so what about the _Object interface?
			//Would need to do this for IDispatch (via GetIDispatchForObject) if it supported it
			//TODO: What about other special interfaces automatically supported like:
			//	ITypeInfo, IObjectSafety, ISupportErrorInfo, IMarshal, IReflect, IDispatchEx, IEnumVARIANT, etc.?
		}

		private int ReleaseDetour(IntPtr unknown)
		{
			var referenceCount = originalRelease(unknown);
			if (referenceCount == 0)
				comObject.Dispose();
			return referenceCount;
		}

		private void InstallHook(IntPtr comInterface, IntPtr releaseDetourPointer)
		{
			var vtable = Marshal.ReadIntPtr(comInterface);
			var originalReleasePointer = Marshal.ReadIntPtr(vtable, 2 * IntPtr.Size);
			Marshal.WriteIntPtr(vtable, 2 * IntPtr.Size, releaseDetourPointer);
			if (originalRelease == null)
				originalRelease = Marshal.GetDelegateForFunctionPointer<ReleaseDelegate>(originalReleasePointer);
			originalRelease(comInterface);
		}

		public void ReleaseMarshalledDelegate()
		{
			releaseDetour = null;
		}
	}
}
