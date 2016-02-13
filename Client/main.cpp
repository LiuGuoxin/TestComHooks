#include <Windows.h>
#include <objbase.h>
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

void Check(HRESULT hr, const std::string& function)
{
	if (SUCCEEDED(hr))
		return;
	std::ostringstream out;
	out << "hr = " << hr << std::endl
		<< "function = " << function << std::endl
		<< "descriptoin = ";
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
	{
		char* errorMessage = nullptr;
		auto formatResult = ::FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			nullptr,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&errorMessage),
			0,
			nullptr);
		if (formatResult != 0)
			out << errorMessage;
		else
			out << "No extra description found";
		if (errorMessage)
			::LocalFree(errorMessage);
	}
	out << std::endl;
	throw std::runtime_error(out.str());
}

void Run(REFCLSID rclsid)
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
		std::cout << "> About to call Bar..." << std::endl;
		hr = foo->Bar();
		Check(hr, "Foo::Bar");
		std::cout << "> About to release QI reference..." << std::endl;
	}
	std::cout << "> About to release final reference..." << std::endl;
}

int main(int argc, char** argv)
{
	std::cout << "Process is starting" << std::endl;
	try
	{
		auto hr = ::CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		Check(hr, "CoInitializeEx");

		if (argc < 2)
			Run(CppCom::CLSID_FooWrapper);
		else if (argv[1] == std::string{ "c++" })
			Run(CppCom::CLSID_FooNative);
		else if (argv[1] == std::string{ "c#" })
			Run(CsCom::CLSID_Foo);
		else if (argv[1] == std::string{ "wrapper" })
			Run(CppCom::CLSID_FooWrapper);
		else if (argv[1] == std::string{ "custom" })
			Run(CsCom::CLSID_FooCustom);
		else
			std::cout << "WTF?" << std::endl;
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
