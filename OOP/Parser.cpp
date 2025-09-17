#include "Parser.h"
#include <string>
#include <sstream>

using namespace std;

// Default prompt character
string Parser::leadingCharacter = "$";

Parser::Parser(Reader* reader) : myReader(reader) {}

Parser::~Parser() {
    delete myReader;
}

Command* Parser::getNextCommand() {
    // Read next line from input
    string line = myReader->getNextLine();

    // Stop if input ended (EOF or exit)
    if (myReader->endOfRead()) return nullptr;

    return parseCommand(line);
}

string Parser::getLeadingCharacter() {
    // Leading character must be set
    if (leadingCharacter.empty()) {
        throw runtime_error("Leading character is not set.");
    }
    return leadingCharacter;
}

void Parser::setLeadingCharacter(const string& character) {
    // New leading character cannot be empty
    if (character.empty()) {
        throw invalid_argument("Leading character string cannot be empty.");
    }
    leadingCharacter = character;
}

void Parser::truncateLine(string& line) {
    // Limit input line length to 512 characters
    if (line.size() > 512) {
        line = line.substr(0, 512);
    }
}

void Parser::removeLeadingCharacter(string& line, const string& leadingChar) {
    // Remove leading prompt character if present
    if (line.substr(0, leadingChar.size()) == leadingChar) {
        line = line.substr(leadingChar.size());
    }
}

Command* Parser::parseCommand(string line) {
    // Make sure line is within max length
    truncateLine(line);

    // Remove prompt character ('$' by default)
    removeLeadingCharacter(line, leadingCharacter);

    istringstream stream(line);
    string command;

    // First token must be the command
    if (!(stream >> command)) {
        cerr << "Invalid command format: Missing command" << endl;
        return nullptr;
    }

    // Use CommandFactory to build and return the command
    return CommandFactory::createCommand(command, stream);
}
