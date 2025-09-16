#include "Parser.h"
#include <string>
#include <sstream>

using namespace std;


// Define the static variable
string Parser::leadingCharacter = "$";

Parser::Parser(Reader* reader) : myReader(reader) {
	
}

Parser::~Parser() {
	delete myReader;
}

Command* Parser::getNextCommand() {
	string line = myReader->getNextLine();
	if (myReader->endOfRead()) return nullptr; //prob
    return parseCommand(line);
}

string Parser::getLeadingCharacter() {
    if (leadingCharacter.empty()) {
        throw runtime_error("Leading character is not set.");
    }
    return leadingCharacter;
}

// Implementation of setLeadingCharacter
void Parser::setLeadingCharacter(const string& character) {
    if (character.empty()) {
        throw invalid_argument("Leading character string cannot be empty.");
    }
    

    leadingCharacter = character;
}


void Parser::truncateLine(string& line) {
    if (line.size() > 512) {
        line = line.substr(0, 512); // Skrati liniju na 512 karaktera
    }
}

void Parser::removeLeadingCharacter(string& line, const string& leadingChar) {
    if (line.substr(0, leadingChar.size()) == leadingChar) {
        line = line.substr(leadingChar.size());
    }
}

Command* Parser::parseCommand(string line) {
    truncateLine(line); // Skrati liniju ako je duža od 512 karaktera
    removeLeadingCharacter(line, leadingCharacter); // Uklanja `$` sa početka linije

    istringstream stream(line);
    string command;

    // Prva reč mora biti komanda
    if (!(stream >> command)) {
        cerr << "Invalid command format: Missing command" << endl;
        return nullptr;
    }

    // Poziv funkcije createCommand iz CommandFactory
    return CommandFactory::createCommand(command, stream);
}
