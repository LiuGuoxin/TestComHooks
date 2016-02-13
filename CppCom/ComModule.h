#pragma once
#include <atomic>

class ComModule
{
public:
	static ComModule& GetInstance()
	{
		static ComModule instance;
		return instance;
	}

	void AddRef()
	{
		++referenceCount;
	}

	void Release()
	{
		--referenceCount;
	}

	bool CanUnload()
	{
		return referenceCount == 0;
	}

private:
	std::atomic<int> referenceCount = 0;
};
