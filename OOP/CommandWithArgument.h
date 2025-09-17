#pragma once
#include "Command.h"

// Base class for commands that take an argument (string, file, or input)
class CommandWithArgument : public Command {
protected:
    string argument;     // Stores the argument (may be file content, text, etc.)
    Reader* myReader;    // Reader used for handling file or console input

    // Reads argument from keyboard if no other input is provided
    void argumentKeyboardInput();

    // Checks if the argument looks like a file (e.g., ends with .txt)
    bool isFile();

    // Checks if the command type works directly with files (e.g., touch, rm)
    bool isFileCommand();

    // Checks if there is no argument provided
    bool noArgument();

    // Reads data from the reader into the argument string
    void newArgument();

    // Removes surrounding quotes from the input string
    string removeQuotes(const string& input);

public:
    // Constructor that initializes command with name and argument
    CommandWithArgument(const string& name, const string& arg);

    // Virtual destructor to ensure proper cleanup of derived classes
    virtual ~CommandWithArgument();

    // Returns the current argument
    string getArgument() const;

    // Sets the argument value explicitly
    void setArgument(const string& newArgument);

private:
        bool isQuoted(const std::string& input) const; // Checks if argument was originally quoted
};
