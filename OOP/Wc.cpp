#include "Wc.h"
#include <iostream>

Wc:: Wc(const std::string& name, const std::string& option, const std::string& argument)
    : CommandOption(name,option,argument), counter(0) {
    //da li  ovde moze da se zove count??
}

void Wc::execute() {
    cout << this->counter << endl;
}
