#pragma once
#include "CommandOption.h"
#include <cctype>

class Wc : public CommandOption {
public:
    Wc(const string& name, const string& option, const string& argument);
    void execute() override;
    

protected:
    virtual void count() = 0;
    int counter;
};


