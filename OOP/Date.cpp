#define _CRT_SECURE_NO_WARNINGS
#include "Date.h"

Date::Date(const std::string& name) : Command(name) {}

void Date::execute() {
    // Get the current system time
    auto now = chrono::system_clock::now();
    time_t nowTime = chrono::system_clock::to_time_t(now);

    // Convert to local time
    struct tm* localTime = localtime(&nowTime);

    // Print formatted date (YYYY-MM-DD)
    *output << put_time(localTime, "%Y-%m-%d ") << endl;
}
