#include "Truncate.h"
#include "MyExceptions.h"
Truncate::Truncate(const string& name, const string& argument) : CommandWithArgument(name, argument) {}


void Truncate::execute()
{
    // Provera da li fajl postoji
   
        ifstream existingFile(argument);
        if (!existingFile.is_open()) {
            throw FileException("Truncate: fajl '" + argument + "' ne postoji.");
        }
        existingFile.close();

        ofstream file(argument, ofstream::trunc);
        if (!file) {
            throw FileException("Truncate: nije moguće otvoriti fajl '" + argument + "'.");
        }

        *output << "Fajl '" << argument << "' uspešno skraćen." << endl;
        //ovo ne treba output nego cout i kod touch  i rm 
    
}
