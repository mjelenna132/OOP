#include "Simulation.h"

Simulation::Simulation() {
    // Create a parser that reads from the console
    myParser = new Parser(new ConsoleReader());
}

Simulation::~Simulation() {
    // Clean up parser when simulation is destroyed
    if (myParser) {
        delete myParser;
        myParser = nullptr;
    }
}

bool Simulation::nextCommand() {
    // Show the current prompt character
    cout << Parser::getLeadingCharacter();

    Command* command = nullptr;

    try {
        // Ask parser for the next command
        command = myParser->getNextCommand();

        if (command) {
            command->execute();  // run the command
            //cout << endl;
            delete command;       // free memory
            return true;          // continue loop
        }
    }
    catch (const std::exception& e) {
        // Handle known errors
        cerr << "Error: " << e.what() << endl;
        if (command) {
            delete command;
            command = nullptr;
        }
        return true; // keep running
    }
    catch (...) {
        // Handle any unknown errors
        cerr << "Unknown error!" << endl;
        if (command) {
            delete command;
            command = nullptr;
        }
        return true; // keep running
    }

    // No command available (EOF or exit)
    return false;
}
