#include "Touch.h"
#include <iostream>
#include <fstream>
#include "MyExceptions.h"

using namespace std;

Touch::Touch(const std::string& name, const std::string& arg)
    :  CommandWithArgument(name, arg) {}

void Touch::execute() {
    if (this->noArgument()) {
        throw ArgumentException("Nije dato ime fajla.");
        return;
    }

  
    ifstream existingFile(argument);
    if (existingFile.is_open()) {
        existingFile.close();
        throw FileException("Fajl '" + argument + "' ve? postoji.");
    }

  
    ofstream newFile(argument);
    if (!newFile) {
        throw FileException("Nije mogu?e kreirati fajl '" + argument + "'.");
    }

   *output << "File '" << argument << "' created successfully." << endl;
}
