#include "Command.h"
#include "CommandFactory.h"


Command::Command(const string& name)
    : commandName(name) {
    this->setOutput();

}

Command::~Command() {
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
            // Dodavanje na kraj fajla (append)
            ofs = new ofstream(CommandFactory::outputFile, ios::app);
        }
        else {
            // Pisanje u fajl (overwrite)
            ofs = new ofstream(CommandFactory::outputFile, ios::trunc);
        }

        if (ofs->is_open()) {
            output = ofs;
        }
        else {
            cerr << "Greška: nije moguće otvoriti fajl \""
                << CommandFactory::outputFile
                << "\" za upis." << endl;
            delete ofs;
            output = &cout;
        }
    }
    else {
        output = &cout;
    }
}
