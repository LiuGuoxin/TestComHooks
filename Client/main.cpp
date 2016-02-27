#include <Windows.h>
#include <objbase.h>
#include <DispEx.h>
#include <atlbase.h>

#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include <stdexcept>
#include <locale>
#include <codecvt>

#import "c:\save\code\tests\TestComHooks\CppCom\Debug\CppCom.tlb" raw_interfaces_only named_guids
#import "c:\save\code\tests\TestComHooks\CsCom\bin\Debug\CsCom.tlb" raw_interfaces_only named_guids

#if defined(ReportEvent)
#undef ReportEvent
#endif
#import "mscorlib.tlb" raw_interfaces_only named_guids

std::string FormatError(int error)
{
	std::string result;
	char* errorMessage = nullptr;
	auto formatResult = ::FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&errorMessage),
		0,
		nullptr);
	if (formatResult != 0)
		result = errorMessage;
	else
		result = "No extra description found";
	if (errorMessage)
		::LocalFree(errorMessage);
	return result;
}

void Check(HRESULT hr, const std::string& function)
{
	if (SUCCEEDED(hr))
		return;
	std::ostringstream out;
	out << "hr = " << hr << std::endl
		<< "function = " << function << std::endl
		<< "description = ";
	CComPtr<IErrorInfo> errorInfo;
	auto errorInfoResult = ::GetErrorInfo(0, &errorInfo);
	if (errorInfoResult == S_OK)
	{
		CComBSTR description;
		auto descriptionResult = errorInfo->GetDescription(&description);
		if (SUCCEEDED(descriptionResult))
		{
			if (description)
				out << std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(description);
			else
				out << "ErrorInfo was blank";
		}
		else
			out << "GetDescription failed! hr = " << descriptionResult;
	}
	else
		out << FormatError(hr);
	out << std::endl;
	throw std::runtime_error(out.str());
}

template <typename Interface>
void TestInterface(const std::string& name, CComPtr<IUnknown>& unknown)
{
	std::cout << "> QI " << name << "...";
	CComPtr<Interface> other;
	auto hr = unknown.QueryInterface(&other);
	std::cout << (SUCCEEDED(hr) ? "supported" : "not-supported") << std::endl;
}

void TestInterfaces(CComPtr<IUnknown>& unknown)
{
	TestInterface<IDispatch>("IDispatch", unknown);
	TestInterface<ITypeInfo>("ITypeInfo", unknown);
	TestInterface<IProvideClassInfo>("IProvideClassInfo", unknown);
	TestInterface<IConnectionPoint>("IConnectionPoint", unknown);
	TestInterface<IConnectionPointContainer>("IConnectionPointContainer", unknown);
	TestInterface<IDispatchEx>("IDispatchEx", unknown);
	TestInterface<IEnumVARIANT>("IEnumVARIANT", unknown);
	TestInterface<mscorlib::IDisposable>("IDisposable", unknown);
	TestInterface<mscorlib::_Object>("_Object", unknown);
	TestInterface<mscorlib::IEnumerable>("IEnumerable", unknown);
}

void Run(REFCLSID rclsid)
{
	CComPtr<ISupportErrorInfo> supportErrorInfo;
	{
		std::cout << "> About to create Foo via unknown..." << std::endl;
		CComPtr<IUnknown> unknown;
		auto hr = unknown.CoCreateInstance(rclsid, nullptr, CLSCTX_INPROC_SERVER);
		Check(hr, "CoCreateInstance()");
		std::cout << "> About to QI to IFoo..." << std::endl;
		{
			CComPtr<CppCom::IFoo> foo;
			hr = unknown.QueryInterface(&foo);
			Check(hr, "QueryInterface");

			std::cout << "> About to QI to ISupportErrorInfo..." << std::endl;
			hr = foo.QueryInterface(&supportErrorInfo);
			std::cout << (SUCCEEDED(hr) ? "supported" : "not-supported") << std::endl;

			TestInterfaces(unknown);

			std::cout << "> About to call Bar..." << std::endl;
			hr = foo->Bar();
			Check(hr, "Foo::Bar");
			std::cout << "> About to release IFoo reference..." << std::endl;
		}
		std::cout << "> About to release IUnknown reference..." << std::endl;
	}
	std::cout << "> About to release ISupportErrorInfo reference..." << std::endl;
}

void RunLate(REFCLSID rclsid)
{
	CComPtr<ISupportErrorInfo> supportErrorInfo;
	{
		std::cout << "> About to create Foo via unknown..." << std::endl;
		CComPtr<IUnknown> unknown;
		auto hr = unknown.CoCreateInstance(rclsid, nullptr, CLSCTX_INPROC_SERVER);
		Check(hr, "CoCreateInstance()");
		std::cout << "> About to QI to IFooLate..." << std::endl;
		{
			CComPtr<CppCom::IFooLate> foo;
			hr = unknown.QueryInterface(&foo);
			Check(hr, "QueryInterface");

			std::cout << "> About to QI to ISupportErrorInfo..." << std::endl;
			hr = foo.QueryInterface(&supportErrorInfo);
			std::cout << (SUCCEEDED(hr) ? "supported" : "not-supported") << std::endl;

			TestInterfaces(unknown);

			std::cout << "> About to call Bar..." << std::endl;
			hr = foo->BarLate();
			Check(hr, "Foo::BarLate");
			std::cout << "> About to release IFooLate reference..." << std::endl;
		}
		std::cout << "> About to release IUnknown reference..." << std::endl;
	}
	std::cout << "> About to release ISupportErrorInfo reference..." << std::endl;
}

void Help()
{
	std::cout
		<< "c++      - C++ FooNative" << std::endl
		<< "late     - C++ FooLate" << std::endl
		<< "c#       - C# Foo" << std::endl
		<< "wrapper  - C++ FooWrapper -> C# Foo" << std::endl
		<< "custom   - C# FooCustom (ICustomQueryInterface)" << std::endl
		<< "hook     - C# FooHook (detour Release)" << std::endl
		<< "hook2    - C# FooHook2 (detour Release/QueryInterface)" << std::endl;
}

int main(int argc, char** argv)
{
	std::cout << "Process is starting" << std::endl;
	try
	{
		auto hr = ::CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		Check(hr, "CoInitializeEx");

		if (argc < 2)
			Help();
		else if (argv[1] == std::string{ "c++" })
			Run(CppCom::CLSID_FooNative);
		else if (argv[1] == std::string{ "late" })
			RunLate(CppCom::CLSID_FooLate);
		else if (argv[1] == std::string{ "c#" })
			Run(CsCom::CLSID_Foo);
		else if (argv[1] == std::string{ "wrapper" })
			Run(CppCom::CLSID_FooWrapper);
		else if (argv[1] == std::string{ "custom" })
			Run(CsCom::CLSID_FooCustom);
		else if (argv[1] == std::string{ "hook" })
			Run(CsCom::CLSID_FooHook);
		else if (argv[1] == std::string{ "hook2" })
			Run(CsCom::CLSID_FooHook2);
		else
			Help();
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unhandled exception" << std::endl;
	}
	std::cout << "Uninitializing COM" << std::endl;
	::CoUninitialize();
	std::cout << "Process is stopping" << std::endl << std::endl;
	return 0;
}
