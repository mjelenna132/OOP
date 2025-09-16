#pragma once
#include "Command.h"
#include "CommandWithArgument.h"
class Batch : public CommandWithArgument {
public:
	Batch(const string& name, const string& argument);
	void execute() override;
};


