
#include "ThreadsafeSingletone.h"
#include <chrono>

SingletoneI* SingletoneI::__instance = nullptr;
std::mutex SingletoneI::__lockCreate;


void createSingletoneI()
{
	std::cout << "\nin thread: " << std::this_thread::get_id();
	SingletoneI::getInstance();
}


void test_CreateSingletoneI()
{
	std::thread th1(createSingletoneI), th2(createSingletoneI);
	std::this_thread::sleep_for(std::chrono::nanoseconds(1000000));
	std::thread th3(createSingletoneI), th4(createSingletoneI);

	th1.join();
	th2.join();
	th3.join();
	th4.join();
}
