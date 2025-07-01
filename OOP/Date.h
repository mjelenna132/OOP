#pragma once
#include"Command.h"
#include <chrono>
#include <iomanip>
#include <ctime>
class Date: public Command{
public:
	Date(const std::string& name);
	void execute() override;
};

