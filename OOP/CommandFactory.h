#pragma once
#include <string>
#include <sstream>
#include <vector>

#include "Wc.h"
#include "WcWords.h"
#include "WcChar.h"
#include "Echo.h"
#include "Time.h"
#include "Date.h"
#include "Touch.h"
#include "Rm.h"
#include "Truncate.h"
#include "Head.h"
#include "Prompt.h"
#include "Batch.h"
#include "Tr.h"
#include "Pipeline.h"

using namespace std;

class CommandFactory {
public:
    // Redirection kinds used across the project
    enum class RedirKind { NONE, IN, OUT_TRUNC, OUT_APPEND };

    // Build a command from the first word + the rest of the line
    static Command* createCommand(const string& command, istringstream& restOfline);

    // Build a single (non-pipeline) command
    // allowInputRedir / allowOutputRedir control whether < or > are allowed here
    static Command* createSingleCommand(const string& command, istringstream& restOfLine,
        bool allowInputRedir, bool allowOutputRedir);

    // Build a pipeline command from multiple segments split by '|'
    static Command* createPipelineCommand(const vector<string>& segments);

    // Read one argument token from the stream (handles quotes and compact redirs)
    static void readArgument(istringstream& stream, string& argument);

    // Global flags/state for current command parsing
    static bool isInputStream;   // whether '<' was set
    static bool isOutputStream;  // whether '>' or '>>' was set
    static bool pipelineActive;  // true while building a pipeline

    // Filenames for redirection
    static string inputFile;
    static string outputFile;

    // What kind of redirection is active
    static RedirKind inputKind;
    static RedirKind outputKind;

private:
    // Parse and apply redirection tokens from the stream
    static void processRedirections(istringstream& stream,
        bool allowInputRedir, bool allowOutputRedir);

    // Clear redirection state before creating a command
    static void resetRedirections();

    // If 'argument' is a compact redirection (e.g., "<in.txt"), apply and consume it
    static void validateArgument(std::string& argument,
        bool allowInputRedir, bool allowOutputRedir);
};


//NE MOZE I DA IMA ARGUMENT I ULAZNA REDIREKCIJA BACI ERROR
//ISPRAVI TR  TR "JELENA" "A"
//BATCH DA MOZE DA PRIMA ARGUMENT NE SAMO DA SE UNOSI SA KONZOLU
//novi red nebitno?
/*
* $tr "jelena" "a"
^Z
"jelena"
*/

//INvalid characters