#include "Batch.h"
#include "CommandFactory.h"
#include <sstream>
#include <iostream>

Batch::Batch(const string& name, const string& argument) : CommandWithArgument(name, argument) {}


void Batch::execute()
{
  

    // Poziv funkcije createCommand iz CommandFactory
    //return CommandFactory::createCommand(command, stream);
    std::istringstream stream(argument); // Pretvaramo argument u ulazni tok
    std::string commandLine, command;

    while (std::getline(stream, commandLine)) { // Čitanje svake komandne linije
        try {
            std::istringstream streamLine(commandLine);

            if (!(streamLine >> command)) {
                std::cerr << "Invalid command format: Missing command" << std::endl;
            }
            // Kreiramo komandu koristeći CommandFactory
            Command* newCommand = CommandFactory::createCommand(command, streamLine );
            if (newCommand) {
                newCommand->execute();
            }
            delete newCommand; // Ručno oslobađanje memorije
        }
        catch (const std::exception& e) {
            // Ispisujemo poruku o grešci i nastavljamo dalje
            std::cerr << "Error executing command: " << commandLine << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }
}
