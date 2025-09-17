#pragma once
#include <iostream>
#include <string>
#include "Reader.h" 
using namespace std;

class Command {
protected:
    string commandName;
    // Default input: keyboard
    ostream* output; // Default output: console

public:
    Command(const string& name);
    virtual ~Command();

    string getCommandName() const;

    // Enable piping and redirection
    virtual void setOutput();

    virtual void execute() = 0; // Abstract method
};
