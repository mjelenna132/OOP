#include "Simulation.h"

Simulation::Simulation() {
    myParser = new Parser(new ConsoleReader());
}

Simulation::~Simulation() {
    if (myParser) {
        delete myParser;
        myParser = nullptr;
    }
}

bool Simulation::nextCommand() {
    cout << Parser::getLeadingCharacter();
    Command* command = nullptr;

    try {
        command = myParser->getNextCommand();
        if (command) {
            command->execute();
            delete command;
            return true; // komanda izvršena, ?ekamo slede?u
        }
    }
    catch (const std::exception& e) {
        cerr << "Greška: " << e.what() << endl;
        // ignoriši komandu, traži novu
        if (command) {
            delete command;
            command = nullptr;
        }
        return true;
    }
    catch (...) {
        cerr << "Nepoznata greška!" << endl;
        if (command) {
            delete command;
            command = nullptr;
        }
        return true;
    }

    return false; // ako nije bilo komande (EOF ili exit)
}
