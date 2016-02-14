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
	This will create a C# FooCustom CMO object that implements ICustomQueryInterface to try to gain access to reference counts.
	This demonstrates improper disposal of the C# object by disposing before the final reference is released.


Diagnosing problems
===================
If you "get class not registered" errors when running client.exe it may be due to the activation context cache.
There is limited documentation on this even existing:
http://blogs.msdn.com/b/junfeng/archive/2007/10/01/vista-activation-context-cache.aspx
The easiest way other than rebooting is to "touch" the client.exe.manifest to cause it to be re-scanned.
