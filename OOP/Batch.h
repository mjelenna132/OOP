#pragma once
#include "Command.h"
#include "CommandWithArgument.h"

// Represents a 'batch' command that executes a sequence of commands
class Batch : public CommandWithArgument {
public:
    // Constructor: initializes the batch command with a name and an argument (e.g., a file path)
    Batch(const string& name, const string& argument);

    // Executes the batch command (reads commands from the file/argument and runs them)
    void execute() override;
};
