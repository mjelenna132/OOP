#define _CRT_SECURE_NO_WARNINGS
#include "Time.h"

Time::Time(const std::string& name) : Command(name) {}

void Time::execute() {
    // Get the current system time
    auto now = chrono::system_clock::now();
    time_t nowTime = chrono::system_clock::to_time_t(now);

    // Convert to local time
    struct tm* localTime = localtime(&nowTime);

    // Print formatted time (HH:MM:SS)
    *output << put_time(localTime, "%H:%M:%S") << endl;
}
