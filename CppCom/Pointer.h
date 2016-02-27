#pragma once
#include <atlbase.h>

namespace Com
{
	template <typename Interface>
	using Pointer = ATL::CComPtr<Interface>;

	template <typename Interface>
	using QueryPointer = ATL::CComQIPtr<Interface>;
}
