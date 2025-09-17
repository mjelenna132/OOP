#pragma once
#include "Command.h"
#include "CommandWithArgument.h"

// Represents an 'echo' command that outputs the given argument to the console or redirected output
class Echo: public CommandWithArgument{
public:

	Echo(const string& name, const string& argument);

	void execute() override;
};

