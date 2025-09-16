#include "Wc.h"
#include <iostream>
#include "MyExceptions.h"
Wc:: Wc(const string& name, const string& option, const string& argument)
    : CommandOption(name,option,argument), counter(0) {
    if (option != "c" && option != "w") {
        throw ArgumentException("Neispravna opcija za wc komandu.");
    }
    //da li  ovde moze da se zove count??
}

void Wc::execute() {
    if (getCommandName() == "head") {
        return;
    }
    count();
    *output << this->counter << endl;
}
