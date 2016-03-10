#pragma once
#include <objbase.h>

namespace Com
{
	class __declspec(uuid("C1E6B0C5-68DB-4A7A-963B-E4D10BA04384")) IClrFactory : public IUnknown
	{
	public:
		virtual HRESULT __stdcall Create(LPCSTR typeName, IUnknown* outer, IUnknown** retval) = 0;
	};
}
