#include <iostream>
#include <chrono>
#include <ctime>
#include <unistd.h>

int main()
{
    const auto p0 = std::chrono::time_point<std::chrono::system_clock>{};
    const auto p1 = std::chrono::system_clock::now();
    const auto p2 = p1 - std::chrono::hours(24);
 
    std::time_t epoch_time = std::chrono::system_clock::to_time_t(p0);
    std::cout << "epoch: " << std::ctime(&epoch_time);
    std::time_t today_time = std::chrono::system_clock::to_time_t(p1);
    std::cout << "today: " << std::ctime(&today_time);
 
    std::cout << "hours since epoch: "
              << std::chrono::duration_cast<std::chrono::hours>(
                   p1.time_since_epoch()).count() 
              << '\n';
    std::cout << "yesterday, hours since epoch: "
              << std::chrono::duration_cast<std::chrono::hours>(
                   p2.time_since_epoch()).count() 
              << '\n';
    
    std::chrono::time_point<std::chrono::system_clock> now = 
        std::chrono::system_clock::now();
    usleep(15111); // 15 miliseconds + 111 microseconds
    std::chrono::time_point<std::chrono::system_clock> now2 = 
        std::chrono::system_clock::now();
    auto duration = now2 - now;
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::cout << millis << std::endl;
}
