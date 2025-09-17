#pragma once
#include "CommandOption.h"

class Head : public CommandOption {
private:
    int lineCount; // how many lines should be displayed

public:
    Head(const string& name, const string& option, const string& argument);
    void execute() override; // outputs the first N lines from the input
};
