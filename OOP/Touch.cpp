#include "Touch.h"
#include <iostream>
#include <fstream>
#include "MyExceptions.h"

using namespace std;

Touch::Touch(const std::string& name, const std::string& arg)
    : CommandWithArgument(name, arg) {}

void Touch::execute() {
    // Check if filename was provided
    if (this->noArgument()) {
        throw ArgumentException("No file name given.");
        return;
    }

    // Check if file already exists
    ifstream existingFile(argument);
    if (existingFile.is_open()) {
        existingFile.close();
        throw FileException("File '" + argument + "' already exists.");
    }

    // Try to create a new file
    ofstream newFile(argument);
    if (!newFile) {
        throw FileException("Could not create file '" + argument + "'.");
    }

    // Report success (respects output redirection)
    *output << "File '" << argument << "' created successfully." << endl;
}
