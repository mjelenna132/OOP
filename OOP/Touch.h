#pragma once
#include "Command.h"
#include"CommandWithArgument.h"
class Touch : public CommandWithArgument {
public:
	Touch(const string& name, const string& argument);
	void execute() override;
};
