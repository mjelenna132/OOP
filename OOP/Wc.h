#pragma once
#include "CommandOption.h"
#include <cctype>

// Represents the 'wc' command (word count) with options
class Wc : public CommandOption {
public:
    // Constructor: initializes the wc command with name, option, and argument
    Wc(const string& name, const string& option, const string& argument);

    // Executes the wc command (entry point for the operation)
    void execute() override;

protected:
    // Abstract method for counting (words, lines, or characters)
    // Must be implemented by derived classes
    virtual void count() = 0;

    // Holds the result of the counting operation
    int counter;
};
