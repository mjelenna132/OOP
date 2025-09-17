#include "WcWords.h"

WcWords::WcWords(const string& name, const string& option, const string& argument)
    : Wc(name, option, argument) {
    
}

void WcWords::count() {
    bool inWord = false;

    // Loop through every character in the argument
    for (char c : argument) {
        if (isspace(c)) {
            // End of a word when we hit space and we were inside a word
            if (inWord) {
                counter++;
                inWord = false;
            }
        }
        else {
            // Non-space character - we are inside a word
            inWord = true;
        }
    }

    // If the text ends while still in a word, count the last word
    if (inWord) {
        counter++;
    }
}
