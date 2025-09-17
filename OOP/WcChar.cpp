#include "WcChar.h"

WcChar::WcChar(const std::string& name, const std::string& option, const std::string& argument)
    : Wc(name, option, argument) {
  
}

void WcChar::count() {
    // Count all characters except newline
    for (char c : argument) {
        if (c != '\n') {
            counter++;
        }
    }
}
