#include "CommandWithArgument.h"

CommandWithArgument::CommandWithArgument(const std::string& name, const std::string& arg)
    : Command(name), argument(arg), myReader(nullptr) {

    // Skidanje navodnika
    if (!argument.empty() && argument.front() == '"' && argument.back() == '"') {
        argument = argument.substr(1, argument.size() - 2);
    }
    // Ucitavanje argumenta iz fajla ako nije komanda touch
    else if (this->isFile() && getCommandName() != "touch") {
        this->argumentFromFile();
    }
    // Ucitavanje sa tastature
    else if (this->noArgument()) {
        this->argumentKeyboardInput();
    }
}

CommandWithArgument::~CommandWithArgument() {
    delete myReader;
}




bool CommandWithArgument::isFile() {
    if (argument.size() < 4) {
        return false;  // Ako je string kra?i od 4 karaktera, ne može završavati na ".txt"
    }
    return argument.substr(argument.size() - 4) == ".txt";

}

bool CommandWithArgument::noArgument() {
    if (argument.empty()) {
        return true;
    }
    return false;
}

void CommandWithArgument::newArgument() {
    string content;
    while (!myReader->endOfRead()) {
        content += myReader->getNextLine();
        if (!myReader->endOfRead()) {
            content += '\n';  // Dodajemo novi red osim na kraju fajla
        }
    }

    argument = content;
}

std::string CommandWithArgument::getArgument() const {
    return argument;
}

void CommandWithArgument::argumentFromFile() {
    myReader = new FileReader(argument);
    this->newArgument();
}



void CommandWithArgument::argumentKeyboardInput() {
    myReader = new ConsoleReader();
    this->newArgument();

}