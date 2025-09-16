#pragma once
#include "Command.h"
#include <vector>
//#include <string>
static bool pipelineActive;

class Pipeline : public Command {
public:
    Pipeline(const vector<Command*>& commands);
    ~Pipeline();

    void execute() override;

private:
    vector<Command*> pipelineCommands;

    string executeSingle(Command* cmd, const string& input);
    
};
