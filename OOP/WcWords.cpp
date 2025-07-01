#include "WcWords.h"


WcWords::WcWords(const std::string& name, const std::string& option, const std::string& argument)
	: Wc(name, option, argument) {
	count();
}
void WcWords::count() {
    bool inWord = false; 
    for (char c : argument) {
        if (isspace(c)) {
  
            if (inWord) {
                counter++;
                inWord = false; // Zavrsavamo rec
            }
        }
        else {
            // Ako naidjemo na slovo ili karakter koji nije razmak, ulazimo u rec
            inWord = true;
        }
    }
    // Provera da li je poslednja rec zavrsena
    if (inWord) {
        counter++;
    }
}
