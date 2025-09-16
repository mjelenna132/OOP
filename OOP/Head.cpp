#include "Head.h"
#include "sstream"
#include "MyExceptions.h"


/*#include <iostream>
#include <string>
#include <stdexcept>
*/

Head::Head(const string& name, const string& option, const string& argument)
    : CommandOption(name, option, argument), lineCount(0)
{
    // PRAVILO: Opcija (nakon što je CommandFactory skinuo crticu '-')
    // MORA počinjati sa 'n' i imati bar jedan karakter posle toga.

    if (option.empty() || option[0] != 'n' || option.length() == 1) {
        throw ArgumentException("Neispravan format opcije za 'head'. Očekivani format je -n<broj>.");
        //this->lineCount = 0; // Postavljamo na 0 da ne bi ispisao ništa
        //return; // Izlazimo iz konstruktora
    }

    // Korišćenje camelCase konvencije za ime promenljive
    string numberPart = option.substr(1);

    /*try {
        this->lineCount = stoi(numberPart);

        // Proveravamo da li je broj negativan, što je takođe neispravno
        if (lineCount < 0) {
            throw std::invalid_argument("Negativan broj redova nije dozvoljen.");
        }
    }
    catch (const std::exception& e) {
        cerr << "Greška: Neispravan format opcije za 'head'. Očekivani format je -n<broj>." << endl;
        this->lineCount = 0;
    }
    */
    try {
        this->lineCount = stoi(numberPart);
    }
    catch (const std::exception& e) {
        throw ArgumentException("Neispravan broj u opciji za 'head'.");
    }

    if (lineCount <= 0) {
        throw ArgumentException("Broj redova mora biti veći od 0.");
    }

}

void Head::execute() {
    

    istringstream stream(argument);
    string line;
    int currentLine = 0;

    // Čitanje i ispisivanje prvih 'lineCount' redova
    while (currentLine < lineCount && getline(stream, line)) {
        *output << line << endl;
        currentLine++;
    }
}