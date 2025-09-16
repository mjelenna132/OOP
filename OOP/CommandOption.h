#pragma once
#include"CommandWithArgument.h"
class CommandOption :public CommandWithArgument {
protected:
    string option;

public:
    CommandOption(const string& name, const string& opt, const string& argument );
    string getOption() const;
};
