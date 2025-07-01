#pragma once
#include"Wc.h"
#include"Command.h"

class WcChar: public Wc{
public:
	WcChar(const string& name, const string& option, const string& argument);
protected:
	void count() override;
};

