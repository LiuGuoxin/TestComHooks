using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace CsCom
{
	public class ReleaseHooker
	{
		private readonly List<ReleaseHook> hooks = new List<ReleaseHook>();

		public ReleaseHooker(IDisposable comObject, params Type[] interfaces)
		{
			var unknown = Marshal.GetIUnknownForObject(comObject);
			hooks.Add(new ReleaseHook(comObject, unknown));
			AddHookAndRelease(comObject, Marshal.GetComInterfaceForObject(comObject, typeof(IDisposable)));
			foreach (var @interface in interfaces)
				AddHookAndRelease(comObject, Marshal.GetComInterfaceForObject(comObject, @interface));
			//Cannot hook this one because it is in write-protected memory
			//AddHookAndRelease(comObject, QueryInterface(unknown, new Guid("df0b3d60-548f-101b-8e65-08002b2bd119"))); //ISupportErrorInfo
			Marshal.Release(unknown);

			//TODO: GetComInterfaceForObject does not work for typeof(object), so what about the _Object interface?
			//Would need to do this for IDispatch (via GetIDispatchForObject) if it supported it
			//TODO: What about other special interfaces automatically supported like:
			//	ITypeInfo, IObjectSafety, ISupportErrorInfo, IMarshal, IReflect, IDispatchEx, IEnumVARIANT, etc.?
		}

		//private IntPtr QueryInterface(IntPtr unknown, Guid iid)
		//{
		//	IntPtr comInterface;
		//	var result = Marshal.QueryInterface(unknown, ref iid, out comInterface);
		//	if (result != 0)
		//		throw new InvalidOperationException($"Object does not support interface {iid}.");
		//	return comInterface;
		//}

		private void AddHookAndRelease(IDisposable comObject, IntPtr comInterface)
		{
			hooks.Add(new ReleaseHook(comObject, comInterface));
			Marshal.Release(comInterface);
		}

		public void ReleaseMarshalledDelegates()
		{
			foreach (var hook in hooks)
				hook.ReleaseMarshalledDelegate();
		}
	}
}
