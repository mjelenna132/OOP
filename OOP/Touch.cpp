#include "Touch.h"
#include <iostream>
#include <fstream>

using namespace std;

Touch::Touch(const std::string& name, const std::string& arg)
    :  CommandWithArgument(name, arg) {}

void Touch::execute() {
    if (this->noArgument()) {
        cout << "Error: No file name provided." << endl;
        return;
    }

  
    ifstream existingFile(argument);
    if (existingFile.is_open()) {
        cout << "Error: File '" << argument << "' already exists." << endl;
        return;
    }

  
    ofstream newFile(argument);
    if (!newFile) {
        cout << "Error: Could not create file '" << argument << "'." << endl;
        return;
    }

    cout << "File '" << argument << "' created successfully." << endl;
}
