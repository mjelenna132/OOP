#pragma once
#include "Reader.h"
#include "CommandFactory.h"

// Parser is responsible for reading input lines (from a Reader),
// cleaning them up (truncate, remove prompt), and converting them
// into Command objects using the CommandFactory.
class Parser {
public:
    Parser(Reader* reader);
    ~Parser();

    // Get the next command from input (returns nullptr if EOF)
    Command* getNextCommand();

    // Manage the leading character shown as a prompt (e.g., "$")
    static string getLeadingCharacter();
    static void setLeadingCharacter(const string& character);

protected:
    // Parse a single line into a Command object
    Command* parseCommand(string line);

    // Reader used for input (console or file)
    Reader* myReader;

private:
    // Global leading character string
    static string leadingCharacter;

    // Helpers for line processing
    static void truncateLine(string& line);                   // cut line to max length
    static void removeLeadingCharacter(string& line,
        const string& leadingChar); // strip prompt char
};
