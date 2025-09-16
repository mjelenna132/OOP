#include "Pipeline.h"
#include "CommandWithArgument.h"
#include <sstream>
#include <iostream>

Pipeline::Pipeline(const vector<Command*>& commands)
    : Command("Pipeline"), pipelineCommands(commands) {
}

Pipeline::~Pipeline() {
    for (Command* cmd : pipelineCommands) {
        delete cmd;
    }
}

void Pipeline::execute() {
    string pipeContent;
    streambuf* originalCout = cout.rdbuf();

    for (size_t i = 0; i < pipelineCommands.size(); ++i) {
        Command* currentCmd = pipelineCommands[i];

        if (i > 0) {
            CommandWithArgument* cmdWithArg = dynamic_cast<CommandWithArgument*>(currentCmd);

            if (cmdWithArg) {
                // Kastovanje je uspelo, postavi argument
                cmdWithArg->setArgument(pipeContent);
            }
            else {
                // --- NOVI DEO - OBRADA GREŠKE ---
                // Kastovanje NIJE USPELO. To zna?i da je komanda u sredini ili na kraju
                // pipeline-a, a ne može da primi argument. To je greška.

                // Ispisujemo poruku o grešci na cerr (standardni izlaz za greške)
                cerr << "Greska: Komanda '" << currentCmd->getCommandName()
                    << "' ne moze da se nadje u cevovodu." << endl;

                // Obavezno vratimo cout na originalno stanje pre nego što prekinemo izvršavanje
                cout.rdbuf(originalCout);

                // Prekidamo dalje izvršavanje pipeline-a
                return;
            }
        }

        stringstream buffer;
        if (i < pipelineCommands.size() - 1) {
            cout.rdbuf(buffer.rdbuf());
        }
        else {
            cout.rdbuf(originalCout);
        }

        currentCmd->execute();

        if (i < pipelineCommands.size() - 1) {
            pipeContent = buffer.str();
            if (!pipeContent.empty() && pipeContent.back() == '\n') {
                pipeContent.pop_back();
            }
        }
    }

    // Vrati cout na originalno stanje za svaki slu?aj, ako petlja nikad nije ušla u `else`
    cout.rdbuf(originalCout);
}