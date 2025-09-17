#pragma once
#include "Command.h"
#include <vector>

// Pipeline is a special command that connects multiple commands
// together so that the output of one becomes the input of the next.
class Pipeline : public Command {
public:
    Pipeline(const vector<Command*>& commands);
    ~Pipeline();

    // Run all commands in the pipeline in order
    void execute() override;

private:
    // Commands that form the pipeline
    vector<Command*> pipelineCommands;

};
