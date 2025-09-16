#pragma once
#include <fstream>
#include <string>
using namespace std;

class Reader {
public:

	Reader();
	virtual ~Reader();

	virtual string getNextLine() = 0;

	virtual bool endOfRead() = 0;
protected:
	bool eofDetected = false;
};

class FileReader : public Reader {
public:

	FileReader(string filepath);
	~FileReader();

	virtual string getNextLine();
	virtual bool endOfRead();

private:
	ifstream* myInput;
	
};

class ConsoleReader : public Reader {
public:
	ConsoleReader();
	~ConsoleReader();

	virtual string getNextLine();

	virtual bool endOfRead();
	void reset();
//private:
	 // Da li je detektovan EOF, treba u konstrukoru

 
};