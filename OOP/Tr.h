#pragma once
#include "CommandWithArgument.h"

class Tr : public CommandWithArgument {
public:
    Tr(const string& name, const string& argument, const string& what, const string& with);
    void execute() override; // Replaces text in the input

private:
    string whatToReplace;  // String to search for
    string replaceWith;    // String to replace with

    void initializeArguments(const string& arg, const string& what, const string& with);
    string replaceAll(const string& input, const string& what, const string& with) const;
};
