#include "Truncate.h"
#include "MyExceptions.h"

Truncate::Truncate(const string& name, const string& argument)
    : CommandWithArgument(name, argument) {}

void Truncate::execute()
{
    // Check if the file exists
    ifstream existingFile(argument);
    if (!existingFile.is_open()) {
        throw FileException("Truncate: file '" + argument + "' does not exist.");
    }
    existingFile.close();

    // Open the file in truncate mode (clear its content)
    ofstream file(argument, ofstream::trunc);
    if (!file) {
        throw FileException("Truncate: could not open file '" + argument + "'.");
    }

    // Report success
    *output << "File '" << argument << "' truncated successfully." << endl;
}
