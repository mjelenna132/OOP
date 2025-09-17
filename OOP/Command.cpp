#include "Command.h"
#include "CommandFactory.h"

Command::Command(const string& name)
    : commandName(name) {
    this->setOutput(); // Set default output (console or redirected)
}

Command::~Command() {
    // Only delete if output is not pointing to standard console
    if (output != &cout) {
        delete output;
    }
}

string Command::getCommandName() const {
    return commandName;
}

void Command::setOutput() {
    if (CommandFactory::isOutputStream) {
        ofstream* ofs = nullptr;

        using RK = CommandFactory::RedirKind;
        if (CommandFactory::outputKind == RK::OUT_APPEND) {
            // Append to the end of the file
            ofs = new ofstream(CommandFactory::outputFile, ios::app);
        }
        else {
            // Overwrite the file
            ofs = new ofstream(CommandFactory::outputFile, ios::trunc);
        }

        // Check if file was successfully opened
        if (ofs->is_open()) {
            output = ofs;
        }
        else {
            cerr << "Error: could not open file \""
                << CommandFactory::outputFile
                << "\" for writing." << endl;
            delete ofs;
            output = &cout; // fallback to console output
        }
    }
    else {
        // Default: output to console
        output = &cout;
    }
}
