#define _CRT_SECURE_NO_WARNINGS
#include "Date.h"

Date::Date(const std::string& name) : Command(name) {}

void Date::execute() {
    auto now = chrono::system_clock::now();
    time_t nowTime = chrono::system_clock::to_time_t(now);

    struct tm* localTime = localtime(&nowTime);  
    cout << put_time(localTime, "%Y-%m-%d ")<< endl;
}
