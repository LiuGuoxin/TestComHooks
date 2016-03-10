using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace CsCom
{
	[ComImport]
	[Guid("C1E6B0C5-68DB-4A7A-963B-E4D10BA04384")]
	[InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
	public interface IClrFactory
	{
		[MethodImpl(MethodImplOptions.InternalCall, MethodCodeType = MethodCodeType.Runtime)]
		IntPtr Create(
			[MarshalAs(UnmanagedType.LPStr), In] string typeName,
			[In] IntPtr outer);
	}

	[ComVisible(true)]
	[ComDefaultInterface(typeof(IClrFactory))]
	[ClassInterface(ClassInterfaceType.None)]
	public class ClrFactory : IClrFactory
	{
		public IntPtr Create(string typeName, IntPtr outer)
		{
			var type = Type.GetType(typeName);
			if (type == null)
				throw new InvalidOperationException("Invalid type name.");
			return Marshal.CreateAggregatedObject(outer, Activator.CreateInstance(type));
		}
	}
}
