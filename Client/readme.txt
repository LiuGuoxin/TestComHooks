Running the client
==================
1) client.exe
	This will create the C++ FooWrapper COM object that calls through to the C# Foo COM object.
	This demonstrates proper disposal of the inner C# object via the C++ proxy.
2) client.exe c++
	This will create the C++ FooNative COM object.
	This demonstrates proper disposal via native C++ objects but does not use C# for the implementation.
3) client.exe c#
	This will create the C# Foo COM object directly from a C++ client.
	This demonstrates improper disposal of the C# object with Dispose being called after program termination.
