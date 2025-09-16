#include "WcWords.h"


WcWords::WcWords(const string& name, const string& option, const string& argument)
	: Wc(name, option, argument) {
	
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
