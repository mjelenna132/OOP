#pragma once
#include "Command.h"
#include <chrono>
#include <iomanip>
#include <ctime>

// Represents a 'date' command that prints the current system date and time
class Date : public Command {
public:
   
    Date(const string& name);

    void execute() override;
};
