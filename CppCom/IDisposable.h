#pragma once
#include <objbase.h>

MIDL_INTERFACE("805D7A98-D4AF-3F0F-967F-E5CF45312D2C")
IDisposable : public IDispatch
{
public:
	virtual void __stdcall Dispose() = 0;
};
