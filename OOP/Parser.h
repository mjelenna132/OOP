#pragma once
#include "Reader.h"
#include"CommandFactory.h"

//includuj ceo folder
class Parser{
public:
	Parser(Reader* reader);
	~Parser();

	Command* getNextCommand();
	std::string getLeadingCharacter();
	void setLeadingCharacter(const std::string& character);

protected:

	Command* parseCommand(string line) ;

	Reader* myReader;

private:
	string leadingCharacter;
	static void truncateLine(std::string& line);
	static void removeLeadingCharacter(std::string& line, char leadingChar);

};

