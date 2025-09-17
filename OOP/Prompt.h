#pragma once
#include "Parser.h"
#include "CommandWithArgument.h"

class Prompt : public CommandWithArgument {
public:
    Prompt(const string& name, const string& argument);
    void execute() override; // Shows or updates the shell prompt
};
