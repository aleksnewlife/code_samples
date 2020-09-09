#pragma once

#include <mutex>
#include <thread>
#include <iostream>


// Singletone with lazzy initialization
class SingletoneI
{
	static SingletoneI* __instance;
	static std::mutex __lockCreate;
public:

	static SingletoneI* getInstance()
	{
		if (__instance)
		{
			std::cout << "\ngetInstance() simple return. " << std::this_thread::get_id();
			return __instance;
		}
		
		std::lock_guard<std::mutex> lock(__lockCreate);
		std::cout << "\ngetInstance() try create. " << std::this_thread::get_id();
		if (!__instance)
		{
			std::cout << "\ngetInstance() create. " << std::this_thread::get_id();
			__instance = new SingletoneI();
		}
		return __instance;
	}

	SingletoneI()
	{
		std::cout << "\nSingletoneI(). " << std::this_thread::get_id();
	}
};


void test_CreateSingletoneI();