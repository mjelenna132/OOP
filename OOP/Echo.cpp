#include "Echo.h"

Echo::Echo(const string& name, const string& argument) : CommandWithArgument(name, argument){}
void Echo::execute() {
	*output << getArgument()<<endl;
}