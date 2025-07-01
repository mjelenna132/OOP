#include "CommandFactory.h"
#include <iostream>
#include <sstream>
using namespace std;

std::string CommandFactory::readArgument(std::istringstream& stream) {
    string argument;
    stream >> argument;

    // Provera prvog znaka
    if (!argument.empty() && argument[0] == '"' && argument.back()!='"') {
        string quotedArgument;
        char c;

        // U?itaj po?etni navodnik
        stream.get(c);
        quotedArgument += c;

        // U?itava sve do zatvaraju?eg navodnika
        while (stream.get(c)) {
            quotedArgument += c;
            if (c == '"') break;  // Prekid na zatvaraju?em navodniku
        }

        argument += quotedArgument;
    }

    return argument;
}

Command* CommandFactory::createCommand(const std::string& command, std::istringstream& restOfLine) {
    string option, argument;
  
    // U?itaj ostatak linije samo za komande koje zahtevaju opciju ili argument
    if (command == "echo" || command == "touch") {
        
        // U?itaj ostatak linije kao argument (ako postoji)
        argument = readArgument(restOfLine);
    }
    else if (command == "wc") {
        
        string word;
        if (!(restOfLine >> option)) {
            exit(6);
        }
        if (option[0]=='-') {
            option.erase(option.begin());
        }
        argument = readArgument(restOfLine);
   
    }

    // Ispis za debug
    cout << "Command  :" << command << endl;
    cout << "Option   :" << option << endl;
    cout << "Argument :" << argument << endl;

    // Kreiraj odgovaraju?u komandu

    if (command == "wc") {
        if (option == "c") {
            return new WcChar(command, option, argument);
            
        }
        else if (option == "w") {
            
            return new WcWords(command, option, argument);
        }
        else {
            cout << "Invalid option for wc command." << endl;
            return nullptr;
        }
    }
    else if (command == "echo") {
        return new Echo(command, argument);
    }
    else if (command == "time") {
        return new Time(command);
    }
    else if (command == "date") {
        return new Date(command);
    }
    else if (command == "touch") {
        return new Touch(command, argument);
    }
    else {
        cout << "Unrecognized command." << endl;
        return nullptr;
    }
}
