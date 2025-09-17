#include "CommandFactory.h"
#include "Pipeline.h"
#include "MyExceptions.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <istream>
#include <cctype>

using namespace std;

// Static state (global redirection/pipeline flags and files)
bool   CommandFactory::isInputStream = false;
bool   CommandFactory::isOutputStream = false;
bool   CommandFactory::pipelineActive = false;
string CommandFactory::inputFile = "";
string CommandFactory::outputFile = "";

CommandFactory::RedirKind CommandFactory::inputKind = CommandFactory::RedirKind::NONE;
CommandFactory::RedirKind CommandFactory::outputKind = CommandFactory::RedirKind::NONE;

// Private helpers (anonymous namespace)
namespace {

    // One redirection token like '<file', '>', '>>out.txt'
    struct RedirTok {
        CommandFactory::RedirKind kind = CommandFactory::RedirKind::NONE;
        string fname; // filename if present (may be empty)
    };

    // Trim spaces
    inline string trim(const string& s) {
        size_t a = s.find_first_not_of(" \t\r\n");
        if (a == string::npos) return "";
        size_t b = s.find_last_not_of(" \t\r\n");
        return s.substr(a, b - a + 1);
    }

    // Remove surrounding quotes if any
    inline string unquoteIfQuoted(const string& s) {
        if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
            return s.substr(1, s.size() - 2);
        }
        return s;
    }

    // Normalize a filename: trim + unquote
    inline void normalizeFilename(string& s) {
        s = trim(unquoteIfQuoted(trim(s)));
    }

    // Parse redirection operators: <, >, >> (with or without attached filename)
    inline bool parseRedirToken(const string& token, RedirTok& out) {
        out = {};
        if (token.empty()) return false;

        if (token == "<") {
            out.kind = CommandFactory::RedirKind::IN;
            out.fname.clear();
        }
        else if (token.rfind("<", 0) == 0) {
            out.kind = CommandFactory::RedirKind::IN;
            out.fname = token.substr(1); // <file
        }
        else if (token == ">>") {
            out.kind = CommandFactory::RedirKind::OUT_APPEND;
            out.fname.clear();
        }
        else if (token.rfind(">>", 0) == 0) {
            out.kind = CommandFactory::RedirKind::OUT_APPEND;
            out.fname = token.substr(2); // >>file
        }
        else if (token == ">") {
            out.kind = CommandFactory::RedirKind::OUT_TRUNC;
            out.fname.clear();
        }
        else if (token.rfind(">", 0) == 0) {
            out.kind = CommandFactory::RedirKind::OUT_TRUNC;
            out.fname = token.substr(1); // >file
        }
        else {
            return false;
        }

        normalizeFilename(out.fname);
        return true;
    }

    // Apply a redirection to global flags (with validation)
    inline void applyRedir(CommandFactory::RedirKind kind, const string& fname,
        bool allowIn, bool allowOut) {
        using RK = CommandFactory::RedirKind;

        if (kind == RK::IN) {
            if (!allowIn)                      throw ArgumentException("Input redirection '<' not allowed here.");
            if (CommandFactory::isInputStream) throw ArgumentException("Duplicate input redirection.");
            if (fname.empty())                 throw SyntaxException("Missing file for input redirection.");

            CommandFactory::isInputStream = true;
            CommandFactory::inputFile = fname;
            CommandFactory::inputKind = RK::IN;
        }
        else if (kind == RK::OUT_TRUNC || kind == RK::OUT_APPEND) {
            if (!allowOut)                      throw ArgumentException("Output redirection '>' not allowed here.");
            if (CommandFactory::isOutputStream) throw ArgumentException("Duplicate output redirection.");
            if (fname.empty())                  throw SyntaxException("Missing file for output redirection.");

            CommandFactory::isOutputStream = true;
            CommandFactory::outputFile = fname;
            CommandFactory::outputKind = kind;
        }
    }

    // Split a pipeline by '|' while respecting quoted strings
    inline vector<string> splitPipeline(const string& line) {
        vector<string> parts;
        string curr;
        bool inQuotes = false;

        for (char c : line) {
            if (c == '"') {
                inQuotes = !inQuotes;
                curr += c;
            }
            else if (c == '|' && !inQuotes) {
                parts.push_back(trim(curr));
                curr.clear();
            }
            else {
                curr += c;
            }
        }
        if (!curr.empty()) parts.push_back(trim(curr));
        return parts;
    }

}
Command* CommandFactory::createCommand(const string& firstWord, istringstream& restOfline) {
    // Rebuild full input line (command + remaining text)
    string rest;
    getline(restOfline, rest);
    string fullLine = firstWord + rest;

    // Reject forbidden characters when not inside quotes
    bool inQuotes = false;
    for (size_t i = 0; i < fullLine.size(); ++i) {
        if (fullLine[i] == '"') inQuotes = !inQuotes;
        if (!inQuotes && (fullLine[i] == '&' || fullLine[i] == '*' ||
            fullLine[i] == '?' || fullLine[i] == '+')) {
            throw SyntaxException("Forbidden character '" + string(1, fullLine[i]) + "'.");
        }
    }

    // Pipeline or single command?
    vector<string> parts = splitPipeline(fullLine);
    if (parts.size() > 1) {
        return createPipelineCommand(parts);
    }
    else {
        istringstream single(fullLine);
        string cmd;
        single >> cmd;
        pipelineActive = false;
        return createSingleCommand(cmd, single, true, true);
    }
}

Command* CommandFactory::createPipelineCommand(const vector<string>& segments) {
    vector<Command*> cmds;
    for (size_t i = 0; i < segments.size(); ++i) {
        bool allowIn = (i == 0);
        bool allowOut = (i + 1 == segments.size());

        istringstream s(segments[i]);
        string cmd;
        s >> cmd;

        if (cmd.empty()) {
            for (auto* p : cmds) delete p;
            throw SyntaxException("Empty command in pipeline at position " + to_string(i));
        }

        Command* c = createSingleCommand(cmd, s, allowIn, allowOut);
        cmds.push_back(c);
        if (i == 0) pipelineActive = true;
    }
    pipelineActive = false;
    return new Pipeline(cmds);
}

void CommandFactory::resetRedirections() {
    // Clear global redirection state
    isInputStream = false;
    isOutputStream = false;
    inputFile.clear();
    outputFile.clear();
    inputKind = RedirKind::NONE;
    outputKind = RedirKind::NONE;
}

void CommandFactory::processRedirections(istringstream& stream,
    bool allowInputRedir, bool allowOutputRedir) {
    // Consume only redirection tokens from the stream
    string token;

    while (stream >> token) {
        RedirTok rt;
        if (!parseRedirToken(token, rt)) {
            throw ArgumentException("Unexpected argument '" + token + "'. Redirection expected.");
        }

        if (rt.fname.empty()) {
            // Operator separated from filename: read filename next
            if (!(stream >> rt.fname)) {
                throw SyntaxException("Missing filename for redirection.");
            }
            normalizeFilename(rt.fname);
        }

        applyRedir(rt.kind, rt.fname, allowInputRedir, allowOutputRedir);
    }
}

Command* CommandFactory::createSingleCommand(const string& command, istringstream& restOfLine,
    bool allowInputRedir, bool allowOutputRedir) {
    // Prepare redirection state for this command
    resetRedirections();

    string option, argument;

    // Simple commands with a single argument (possibly quoted)
    if (command == "echo" || command == "touch" || command == "truncate" ||
        command == "rm" || command == "batch" || command == "prompt") {

        readArgument(restOfLine, argument);
        processRedirections(restOfLine, allowInputRedir, allowOutputRedir);

        if (command == "echo")     return new Echo(command, argument);
        if (command == "touch")    return new Touch(command, argument);
        if (command == "rm")       return new Rm(command, argument);
        if (command == "truncate") return new Truncate(command, argument);
        if (command == "prompt")   return new Prompt(command, argument);
        if (command == "batch")    return new Batch(command, argument);
    }
    // Commands with an option + argument
    else if (command == "wc" || command == "head") {
        if (!(restOfLine >> option)) throw ArgumentException("Missing option for '" + command + "'.");
        if (!option.empty() && option[0] == '-') option.erase(option.begin()); // drop leading '-'

        readArgument(restOfLine, argument);
        processRedirections(restOfLine, allowInputRedir, allowOutputRedir);

        if (command == "wc") {
            if (option == "c") return new WcChar(command, option, argument);
            else if (option == "w") return new WcWords(command, option, argument);
            else throw ArgumentException("Unknown option for wc.");
        }
        else {
            return new Head(command, option, argument);
        }
    }
    // 'tr' has up to 3 positional args: [argument] what [with]
    else if (command == "tr") {
        std::vector<std::string> args;

        // Read up to three tokens before redirections
        for (;;) {
            restOfLine >> ws;                 // skip spaces
            int ch = restOfLine.peek();
            if (ch == EOF || ch == '<' || ch == '>') break; // redirs or end

            std::string tok;
            readArgument(restOfLine, tok);    // read token (quoted-safe)
            if (!tok.empty()) args.push_back(tok);

            if (args.size() == 3) break;
        }

        // Then process any redirections
        processRedirections(restOfLine, allowInputRedir, allowOutputRedir);

        // Map tokens into: tr [argument] what [with]
        if (args.empty()) {
            throw ArgumentException("For 'tr', 'what' is required. Usage: tr [argument] what [with].");
        }

        std::string argForTr, what, with;

        if (args.size() == 1) {
            // only WHAT; input will come from keyboard or '<'
            argForTr = "";
            what = args[0];
            with = "";
        }
        else if (args.size() == 2) {
            // Ambiguous: if input is redirected, treat as WHAT, WITH; else ARGUMENT, WHAT
            if (isInputStream) {
                argForTr = "";
                what = args[0];
                with = args[1];
            }
            else {
                argForTr = args[0];
                what = args[1];
                with = "";
            }
        }
        else { // args.size() == 3
            argForTr = args[0];
            what = args[1];
            with = args[2];
        }

        return new Tr(command, argForTr, what, with);
    }
    // Commands without args (time/date), only redirections allowed
    else if (command == "time" || command == "date") {
        restOfLine >> ws;
        int ch = restOfLine.peek();

        if (ch == EOF) {
            // no extra args
        }
        else if (ch == '<' || ch == '>') {
            processRedirections(restOfLine, allowInputRedir, allowOutputRedir);
        }
        else {
            string junk;
            restOfLine >> junk;
            throw ArgumentException("Too many arguments for '" + command + "'.");
        }

        if (command == "time") return new Time(command);
        else                   return new Date(command);
    }
    else {
        throw CommandException("Unknown command.");
    }

    return nullptr;
}

// Do not consume separated redirection operators here; let processRedirections handle them
void CommandFactory::readArgument(istringstream& stream, string& argument) {
    argument.clear();

    // Skip spaces
    stream >> ws;
    int ch1 = stream.peek();
    if (ch1 == EOF) return;

    // If we see a redirection operator, decide if it's separated or attached to a filename
    if (ch1 == '<' || ch1 == '>') {
        char c1; stream.get(c1);
        int ch2 = stream.peek();
        bool isAppendOp = (c1 == '>' && ch2 == '>');
        if (isAppendOp) {
            char c2; stream.get(c2);
            ch2 = stream.peek();
        }

        // Separated operator: put back and return (so processRedirections can handle it)
        if (ch2 == EOF || isspace(ch2)) {
            stream.unget();
            if (isAppendOp) stream.unget();
            stream.unget();
            return;
        }
        else {
            // Attached operator: put back and read as a normal token
            if (isAppendOp) stream.unget();
            stream.unget();
        }
    }

    // Read a normal token (supports quoted tokens)
    if (!(stream >> argument)) return;

    // If opening quote without closing yet, keep reading until closing quote
    if (!argument.empty() && argument.front() == '"' &&
        (argument.size() == 1 || argument.back() != '"')) {
        string tail; char c;
        while (stream.get(c)) {
            tail.push_back(c);
            if (c == '"') break;
        }
        argument += tail;
    }

    // If token is a compact redirection (e.g., "<in.txt", ">>out.txt"), handle it now
    validateArgument(argument, true, true);
}

void CommandFactory::validateArgument(std::string& argument,
    bool allowInputRedir, bool allowOutputRedir) {
    if (argument.empty()) return;

    RedirTok rt;
    if (!parseRedirToken(argument, rt)) return;

    if (!rt.fname.empty()) {
        // Apply redirection and consume this token
        applyRedir(rt.kind, rt.fname, allowInputRedir, allowOutputRedir);
        argument.clear();
    }
}
