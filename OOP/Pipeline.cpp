#include "Pipeline.h"
#include "CommandWithArgument.h"
#include <sstream>
#include <iostream>

Pipeline::Pipeline(const vector<Command*>& commands)
    : Command("Pipeline"), pipelineCommands(commands) {}

Pipeline::~Pipeline() {
    // Free all commands in the pipeline
    for (Command* cmd : pipelineCommands) {
        delete cmd;
    }
}

void Pipeline::execute() {
    string pipeContent;
    streambuf* originalCout = cout.rdbuf(); // Save the original output buffer

    for (size_t i = 0; i < pipelineCommands.size(); ++i) {
        Command* currentCmd = pipelineCommands[i];

        // If not the first command, pass previous output as input
        if (i > 0) {
            CommandWithArgument* cmdWithArg = dynamic_cast<CommandWithArgument*>(currentCmd);

            if (cmdWithArg) {
                // Set argument for the command from the previous output
                cmdWithArg->setArgument(pipeContent);
            }
            else {
                // Command cannot take input - error
                cerr << "Error: Command '" << currentCmd->getCommandName()
                    << "' cannot be used in a pipeline." << endl;

                // Restore original output and stop
                cout.rdbuf(originalCout);
                return;
            }
        }

        // Redirect output: buffer if not the last command, otherwise back to console
        stringstream buffer;
        if (i < pipelineCommands.size() - 1) {
            cout.rdbuf(buffer.rdbuf());
        }
        else {
            cout.rdbuf(originalCout);
        }

        // Run the command
        currentCmd->execute();

        // Save output for the next command
        if (i < pipelineCommands.size() - 1) {
            pipeContent = buffer.str();
            if (!pipeContent.empty() && pipeContent.back() == '\n') {
                pipeContent.pop_back(); // remove trailing newline
            }
        }
    }

    // Restore original output just in case
    cout.rdbuf(originalCout);
}
