#include "Parser.h"
#include <string>
#include <sstream>

using namespace std;

Parser::Parser(Reader* reader) :myReader(reader), leadingCharacter("$") {
}

Parser::~Parser() {
	delete myReader;
}

Command* Parser::getNextCommand() {
	string line = myReader->getNextLine();
	if (myReader->endOfRead()) return nullptr;
    return parseCommand(line);
}

std::string Parser::getLeadingCharacter() {
    if (leadingCharacter.empty()) {
        throw std::runtime_error("Leading character is not set.");
    }
    return leadingCharacter;
}

// Implementation of setLeadingCharacter
void Parser::setLeadingCharacter(const std::string& character) {
    if (character.size() != 1 || !std::isalnum(character[0])) {
        throw std::invalid_argument("Leading character must be a single alphanumeric character.");
    }
    leadingCharacter = character;
}


void Parser::truncateLine(std::string& line) {
    if (line.size() > 512) {
        line = line.substr(0, 512); // Skrati liniju na 512 karaktera
    }
}

void Parser::removeLeadingCharacter(std::string& line, char leadingChar) {
    if (!line.empty() && line.front() == leadingChar) {
        line = line.substr(1); // Uklanja prvi karakter ako se poklapa
    }
}

Command* Parser::parseCommand(std::string line) {
    truncateLine(line); // Skrati liniju ako je duža od 512 karaktera
    removeLeadingCharacter(line, leadingCharacter[0]); // Uklanja `$` sa početka linije

    std::istringstream stream(line);
    std::string command;

    // Prva reč mora biti komanda
    if (!(stream >> command)) {
        std::cerr << "Invalid command format: Missing command" << std::endl;
        return nullptr;
    }

    // Poziv funkcije createCommand iz CommandFactory
    return CommandFactory::createCommand(command, stream);
}
