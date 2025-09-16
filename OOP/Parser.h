#pragma once
#include "Reader.h"
#include"CommandFactory.h"

//includuj ceo folder
class Parser{
public:
	Parser(Reader* reader);
	~Parser();

	Command* getNextCommand();

	static string getLeadingCharacter();
	static void setLeadingCharacter(const string& character);

protected:

	Command* parseCommand(string line) ;

	Reader* myReader;

private:
	static string leadingCharacter;
	static void truncateLine(string& line);
	static void removeLeadingCharacter(string& line, const string& leadingChar);
	 

};

