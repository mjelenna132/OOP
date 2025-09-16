#pragma once
#include "Command.h"
#include"CommandWithArgument.h"
class Truncate : public CommandWithArgument {
public:
	Truncate(const string& name, const string& argument);
	void execute() override;
};

