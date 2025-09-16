#include "Rm.h"
#include "MyExceptions.h"

Rm::Rm(const string& name, const string& argument) : CommandWithArgument(name, argument)
{}

void Rm::execute()
{
    // Provera da li fajl postoji
    ifstream existingFile(argument);
    if (!existingFile.is_open()) {
        throw FileException("Fajl '" + argument + "' ne postoji.");
    }
    existingFile.close(); // Zatvaramo fajl pre brisanja

    // Brisanje fajla
    //funckija prima char tip valjda
    if (remove(argument.c_str())) {
        throw FileException("Nije uspelo brisanje fajla '" + argument + "'.");
    }

    cout << "File '" << argument << "' deleted successfully." << endl;
    // nw treba *output
}
