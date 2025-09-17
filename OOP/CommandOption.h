#pragma once
#include "CommandWithArgument.h"

// Represents a command that supports an additional option parameter
class CommandOption : public CommandWithArgument {
protected:
    string option; // Stores the option provided to the command

public:
    CommandOption(const string& name, const string& opt, const string& argument);
    string getOption() const;
};
