#pragma once
#include "Command.h"
#include"CommandWithArgument.h"
class Rm : public CommandWithArgument {
public:
	Rm(const string& name, const string& argument);
	void execute() override;
};

