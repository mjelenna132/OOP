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
    // Enum za redirekcije (jedini u celom projektu)
    enum class RedirKind { NONE, IN, OUT_TRUNC, OUT_APPEND };

    static Command* createCommand(const string& command, istringstream& restOfline);
    static Command* createSingleCommand(const string& command, istringstream& restOfLine,
        bool allowInputRedir, bool allowOutputRedir);
    static Command* createPipelineCommand(const vector<string>& segments);

    static void readArgument(istringstream& stream, string& argument);

    static bool isInputStream;
    static bool isOutputStream;
    static bool pipelineActive;

    static string inputFile;
    static string outputFile;

    static RedirKind inputKind;
    static RedirKind outputKind;

private:
    static void processRedirections(istringstream& stream, bool allowInputRedir, bool allowOutputRedir);
    static void resetRedirections();
    static void validateArgument(std::string& argument, bool allowInputRedir, bool allowOutputRedir);
};
