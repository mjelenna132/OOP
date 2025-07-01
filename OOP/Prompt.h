#pragma once

#include "CommandWithArgument.h"
class Prompt: public CommandWithArgument{
public:
	Prompt(const string& name, const string& argument);
	void execute() override;
private:
	//Parser* parser;
};

