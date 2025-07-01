#pragma once
#include "Command.h"
#include "CommandWithArgument.h"
class Echo: public CommandWithArgument{
public:
	Echo(const string& name, const string& argument);
	void execute() override;
};

//kad je u vise redova odma mi prekine program