#define _CRT_SECURE_NO_WARNINGS
#include "Time.h"


Time::Time(const std::string& name) : Command(name) {}

void Time::execute() {
    auto now = chrono::system_clock::now();
    time_t nowTime = chrono::system_clock::to_time_t(now);

    struct tm* localTime = localtime(&nowTime);  // Bez promena
    cout << put_time(localTime, "%H:%M:%S") << endl;
}
