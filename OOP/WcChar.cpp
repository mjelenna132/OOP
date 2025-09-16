#include "WcChar.h"

WcChar::WcChar(const std::string& name, const std::string& option, const std::string& argument)
	: Wc(name, option, argument) {
	
	// da li ovo moze da se zove u Wc konstruktoru
}





void WcChar::count(){
	for (char c : argument)
	{
		if (c!='\n') {
			counter++;
		}

	}
}
