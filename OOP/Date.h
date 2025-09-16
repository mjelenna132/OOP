#pragma once
#include"Command.h"
#include <chrono>
#include <iomanip>
#include <ctime>
class Date: public Command{
public:
	Date(const string& name);
	void execute() override;
};

