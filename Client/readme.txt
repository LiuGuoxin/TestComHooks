Running the client
==================
1) client.exe wrapper
	This will create the C++ FooWrapper COM object that calls through to the C# Foo COM object.
	This demonstrates proper disposal of the inner C# object via the C++ proxy.
2) client.exe c++
	This will create the C++ FooNative COM object.
	This demonstrates proper disposal via native C++ objects but does not use C# for the implementation.
3) client.exe c#
	This will create the C# Foo COM object directly from a C++ client.
	This demonstrates improper disposal of the C# object with Dispose being called after program termination.
4) client.exe custom
	This will create a C# FooCustom COM object that implements ICustomQueryInterface to try to gain access to reference counts.
	This demonstrates improper disposal of the C# object by disposing before the final reference is released.
5) client.exe hook
	This will create a C# FooHook COM object that detours the Release v-table entries in all exposed COM interfaces.
	It detects the final release and disposes the object if the final release was on an interface that was detoured.
6) client.exe hook2
	This will create a C# FooHook2 COM object that detours the Release v-table entries in all exposed COM interfaces.
	This differs from hook in that automatically supported interfaces like ISupportErrorInfo are also supported for final release.
	This appears to work in all intended cases.
7) client.exe late
	This will create a C++ FooLate COM object that is just for testing implementing IDispatch from C++.
8) client.exe aggregate
	This will create a C++ FooAggregate COM object that aggregates an inner C# object (not the manual wrapper from 1).
	This works if the C# class uses a different CLSID than the C++ one, but not if they are the same.
	The C++ one cannot resolve the C# one if they use the same CLSID (and resolving by type name performs differently).

Diagnosing problems
===================
If you "get class not registered" errors when running client.exe it may be due to the activation context cache.
There is limited documentation on this even existing:
http://blogs.msdn.com/b/junfeng/archive/2007/10/01/vista-activation-context-cache.aspx
The easiest way other than rebooting is to "touch" the client.exe.manifest to cause it to be re-scanned.
