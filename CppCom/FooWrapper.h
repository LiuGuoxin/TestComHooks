#pragma once
#include "ProxyObject.h"
#include "CppCom_h.h"
#import "c:\save\code\tests\TestComHooks\CsCom\bin\Debug\CsCom.tlb" raw_interfaces_only named_guids

class FooWrapper : public ProxyObject<FooWrapper, IFoo, &CLSID_FooWrapper, &CsCom::CLSID_Foo>
{
public:
	HRESULT __stdcall Bar() final
	{
		return inner->Bar();
	}
};
