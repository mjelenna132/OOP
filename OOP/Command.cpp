#include "Command.h"
#include "CommandOption.h"


// Osnovna klasa Command
Command::Command(const std::string& name) : commandName(name) {
}

Command::~Command() {}

std::string Command::getCommandName() const {
    return commandName;
}



