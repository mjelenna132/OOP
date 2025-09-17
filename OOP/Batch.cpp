#include "Batch.h"
#include "CommandFactory.h"
#include <sstream>
#include <iostream>

using std::string;

namespace {

    // --- Small helpers local to this file ---

    // Trim leading/trailing whitespace
    static inline string trim(const string& s) {
        size_t a = s.find_first_not_of(" \t\r\n");
        if (a == string::npos) return "";
        size_t b = s.find_last_not_of(" \t\r\n");
        return s.substr(a, b - a + 1);
    }

    // Detect if a line already contains an output redirection (>, >>) outside quotes.
    // We keep it simple: any '>' outside quotes means "has its own redirection".
    static bool lineHasOwnOutRedir(const string& line) {
        bool inQuotes = false;
        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (c == '"') {
                inQuotes = !inQuotes;
                continue;
            }
            if (!inQuotes && c == '>') {
                return true;
            }
        }
        return false;
    }

} // namespace

Batch::Batch(const string& name, const string& argument)
    : CommandWithArgument(name, argument) {}

// Behavior:
// - If "batch" itself has an output redirection (e.g., `batch > out.txt`),
//   we want every inner line without its own redirection to inherit that.
// - If "batch" used '>' (truncate), only the FIRST inherited line uses '>',
//   and subsequent inherited lines use '>>' (append), so we don't overwrite.
// - If "batch" used '>>' (append), then ALL inherited lines use '>>'.
// - If an inner line already has its own redirection, we leave it as-is.
void Batch::execute()
{
    // Snapshot the batch-level output redirection (set by CommandFactory before construction).
    const bool   batchHasOut = CommandFactory::isOutputStream;
    const string batchOutFile = CommandFactory::outputFile;
    const auto   batchOutKind = CommandFactory::outputKind;

    // To avoid overwriting when batch used '>', only the first inherited line uses '>'.
    // All subsequent inherited lines will use '>>'.
    bool firstOutApplied = false;

    std::istringstream stream(argument);
    std::string commandLine;

    while (std::getline(stream, commandLine)) {
        try {
            commandLine = trim(commandLine);
            if (commandLine.empty()) continue;

            // If batch has an output redirection and the line doesn't have its own,
            // attach the batch redirection to this line.
            if (batchHasOut && !lineHasOwnOutRedir(commandLine)) {
                bool useAppend =
                    (batchOutKind == CommandFactory::RedirKind::OUT_APPEND) ||
                    firstOutApplied; // after the first, always append

                if (useAppend) {
                    commandLine += " >> " + batchOutFile;
                }
                else {
                    commandLine += " > " + batchOutFile;
                    firstOutApplied = true; // next inherited lines will append
                }
            }

            // Parse: first token is the command name, the rest is its arguments.
            std::istringstream streamLine(commandLine);
            std::string command;
            if (!(streamLine >> command)) {
                std::cerr << "Invalid command format: missing command" << std::endl;
                continue;
            }

            // Create and execute the command via the factory.
            Command* newCommand = CommandFactory::createCommand(command, streamLine);
            if (newCommand) {
                newCommand->execute();
                delete newCommand;
            }
        }
        catch (const std::exception& e) {
            // Don't stop the whole batch on one error; report and continue.
            std::cerr << "Error executing command: " << commandLine << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }
}
