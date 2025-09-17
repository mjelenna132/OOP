#pragma once
#include "Command.h"
#include <chrono>
#include <iomanip>
#include <ctime>

class Time : public Command {
public:
    Time(const std::string& name);
    void execute() override; // Prints current system time
};
