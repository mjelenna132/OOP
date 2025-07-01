#include "Echo.h"

Echo::Echo(const string& name, const string& argument) : CommandWithArgument(name, argument){}
void Echo::execute() {
	cout << getArgument()<<endl;
}