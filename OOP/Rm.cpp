#include "Rm.h"
#include "MyExceptions.h"

Rm::Rm(const string& name, const string& argument)
    : CommandWithArgument(name, argument) {}

void Rm::execute()
{
    // Check if the file exists
    ifstream existingFile(argument);
    if (!existingFile.is_open()) {
        throw FileException("File '" + argument + "' does not exist.");
    }
    existingFile.close(); // Close before removing

    // Delete the file (remove() expects a const char*)
    if (remove(argument.c_str())) {
        throw FileException("Failed to delete file '" + argument + "'.");
    }

    // Report success (could also use *output instead of cout)
    cout << "File '" << argument << "' deleted successfully." << endl;
}
