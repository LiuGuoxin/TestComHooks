using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;

namespace CsCom
{
	public class DeterministicFinalizer : IDisposable
	{
		private bool disposed;
		private readonly CcwWrappers wrappers = new CcwWrappers();
		private readonly List<CcwHook> hooks = new List<CcwHook>();

		public DeterministicFinalizer(IDisposable comObject)
		{
			var unknown = Marshal.GetIUnknownForObject(comObject);
			//TODO: GetIDispatchForObject if any interfaces are ComInterfaceType InterfaceIsDual or InterfaceIsIDispatch.
			var interfacePointers = comObject.GetType()
				.GetInterfaces()
				.Where(Marshal.IsTypeVisibleFromCom)
				.Select(@interface => Marshal.GetComInterfaceForObject(comObject, @interface))
				.ToList();
			var supportErrorInfo = wrappers.Wrap(unknown, new Guid("df0b3d60-548f-101b-8e65-08002b2bd119"), 4);
			var provideClassInfo = wrappers.Wrap(unknown, new Guid("B196B283-BAB4-101A-B69C-00AA00341D07"), 4);
			var connectionPointContainer = wrappers.Wrap(unknown, new Guid("B196B284-BAB4-101A-B69C-00AA00341D07"), 5);
			//TODO: optional (?) IConnectionPoint "B196B286-BAB4-101A-B69C-00AA00341D07" functionCount=8 (3 + 5)
			//TODO: optional (if IExpando) IDispatchEx "A6EF9860-C720-11d0-9337-00A0C90DCAA9" functionCount=15 (3 + 4 + 8)
			//TODO: optional (if IEnumerable) IEnumVARIANT "00020404-0000-0000-C000-000000000046" functionCount=7 (3 + 4)

			hooks.Add(new CcwHook(comObject, unknown, wrappers));
			foreach (var interfacePointer in interfacePointers)
				hooks.Add(new CcwHook(comObject, interfacePointer, wrappers));
			hooks.Add(new CcwHook(comObject, supportErrorInfo, wrappers));
			hooks.Add(new CcwHook(comObject, provideClassInfo, wrappers));
			hooks.Add(new CcwHook(comObject, connectionPointContainer, wrappers));

			Marshal.Release(unknown);
			foreach (var interfacePointer in interfacePointers)
				Marshal.Release(interfacePointer);
			Marshal.Release(supportErrorInfo.Wrapper);
			Marshal.Release(provideClassInfo.Wrapper);
			Marshal.Release(connectionPointContainer.Wrapper);
		}

		~DeterministicFinalizer()
		{
			Dispose(true);
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
			wrappers.Dispose();
			foreach (var hook in hooks)
				hook.ReleaseMarshalledDelegates();
		}
	}
}