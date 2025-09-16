#include "CommandFactory.h"
#include "Pipeline.h"
#include "MyExceptions.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <istream>
#include <cctype>

using namespace std;

// ======================================================================
// Statics
// ======================================================================
bool   CommandFactory::isInputStream = false;
bool   CommandFactory::isOutputStream = false;
bool   CommandFactory::pipelineActive = false;
string CommandFactory::inputFile = "";
string CommandFactory::outputFile = "";

CommandFactory::RedirKind CommandFactory::inputKind = CommandFactory::RedirKind::NONE;
CommandFactory::RedirKind CommandFactory::outputKind = CommandFactory::RedirKind::NONE;

// ======================================================================
// Privatni helper-i
// ======================================================================
namespace {

    struct RedirTok {
        CommandFactory::RedirKind kind = CommandFactory::RedirKind::NONE;
        string fname; // ako nema imena u istom tokenu, ostaje prazno
    };

    inline string trim(const string& s) {
        size_t a = s.find_first_not_of(" \t\r\n");
        if (a == string::npos) return "";
        size_t b = s.find_last_not_of(" \t\r\n");
        return s.substr(a, b - a + 1);
    }

    inline string unquoteIfQuoted(const string& s) {
        if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
            return s.substr(1, s.size() - 2);
        }
        return s;
    }

    inline void normalizeFilename(string& s) {
        s = trim(unquoteIfQuoted(trim(s)));
    }

    // Prepoznaj <, >, >> sa i bez razmaka
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

    // Primeni redirekciju
    inline void applyRedir(CommandFactory::RedirKind kind, const string& fname, bool allowIn, bool allowOut) {
        using RK = CommandFactory::RedirKind;

        if (kind == RK::IN) {
            if (!allowIn)                      throw ArgumentException("Redirekcija ulaza '<' nije dozvoljena.");
            if (CommandFactory::isInputStream) throw ArgumentException("Dvostruka redirekcija ulaza nije dozvoljena.");
            if (fname.empty())                 throw SyntaxException("Fajl za ulaznu redirekciju nije naveden.");

            CommandFactory::isInputStream = true;
            CommandFactory::inputFile = fname;
            CommandFactory::inputKind = RK::IN;
        }
        else if (kind == RK::OUT_TRUNC || kind == RK::OUT_APPEND) {
            if (!allowOut)                      throw ArgumentException("Redirekcija izlaza '>' nije dozvoljena.");
            if (CommandFactory::isOutputStream) throw ArgumentException("Dvostruka redirekcija izlaza nije dozvoljena.");
            if (fname.empty())                  throw SyntaxException("Fajl za izlaznu redirekciju nije naveden.");

            CommandFactory::isOutputStream = true;
            CommandFactory::outputFile = fname;
            CommandFactory::outputKind = kind;
        }
    }

    // Split po |, ignoriši unutar navodnika
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

} // namespace

// ======================================================================
// CommandFactory javne metode
// ======================================================================

Command* CommandFactory::createCommand(const string& firstWord, istringstream& restOfline) {
    string rest;
    getline(restOfline, rest);
    string fullLine = firstWord + rest;

    bool inQuotes = false;
    for (size_t i = 0; i < fullLine.size(); ++i) {
        if (fullLine[i] == '"') inQuotes = !inQuotes;
        if (!inQuotes && (fullLine[i] == '&' || fullLine[i] == '*' || fullLine[i] == '?' || fullLine[i] == '+')) {
            throw SyntaxException("Nedozvoljen karakter '" + string(1, fullLine[i]) + "'.");
        }
    }

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
            throw SyntaxException("Prazna komanda u cevovodu na poziciji " + to_string(i));
        }

        Command* c = createSingleCommand(cmd, s, allowIn, allowOut);
        cmds.push_back(c);
        if (i == 0) pipelineActive = true;
    }
    pipelineActive = false;
    return new Pipeline(cmds);
}

void CommandFactory::resetRedirections() {
    isInputStream = false;
    isOutputStream = false;
    inputFile.clear();
    outputFile.clear();
    inputKind = RedirKind::NONE;
    outputKind = RedirKind::NONE;
}

void CommandFactory::processRedirections(istringstream& stream, bool allowInputRedir, bool allowOutputRedir) {
    string token;

    while (stream >> token) {
        RedirTok rt;
        if (!parseRedirToken(token, rt)) {
            throw ArgumentException("Neočekivani argument '" + token + "'. Očekivana je redirekcija.");
        }

        if (rt.fname.empty()) {
            if (!(stream >> rt.fname)) {
                throw SyntaxException("Fajl za redirekciju nije naveden.");
            }
            normalizeFilename(rt.fname);
        }

        applyRedir(rt.kind, rt.fname, allowInputRedir, allowOutputRedir);
    }
}

Command* CommandFactory::createSingleCommand(const string& command, istringstream& restOfLine,
    bool allowInputRedir, bool allowOutputRedir) {
    resetRedirections();

    string option, argument;

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
    else if (command == "wc" || command == "head") {
        if (!(restOfLine >> option)) throw ArgumentException("Nedostaje opcija za komandu '" + command + "'.");
        if (!option.empty() && option[0] == '-') option.erase(option.begin());

        readArgument(restOfLine, argument);
        processRedirections(restOfLine, allowInputRedir, allowOutputRedir);

        if (command == "wc") {
            if (option == "c") return new WcChar(command, option, argument);
            else if (option == "w") return new WcWords(command, option, argument);
            else throw ArgumentException("Nepoznata opcija za wc komandu.");
        }
        else {
            return new Head(command, option, argument);
        }
    }
    else if (command == "tr") {
        std::vector<std::string> args;
        for (;;) {
            restOfLine >> ws;                 // preskoči beline
            int ch = restOfLine.peek();
            if (ch == EOF || ch == '<' || ch == '>') break; // slede redirekcije ili kraj

            std::string tok;
            readArgument(restOfLine, tok);    // čita i quoted tokene, ne dira redirekcije
            if (!tok.empty()) args.push_back(tok);

            if (args.size() == 3) break;      // maksimalno: [argument] what [with]
        }

        // 2) Obradi redirekcije (spojene i razdvojene)
        processRedirections(restOfLine, allowInputRedir, allowOutputRedir);

        // 3) Mapiranje na: tr [argument] what [with]
        if (args.empty()) {
            throw ArgumentException("Za 'tr' je obavezan 'what'. Dozvoljeno: tr [argument] what [with].");
        }

        std::string argForTr, what, with;

        if (args.size() == 1) {
            // samo WHAT; argument i with izostavljeni
            argForTr = "";            // → CommandWithArgument će čitati sa tastature ili iz < input.txt
            what = args[0];
            with = "";
            cout << "uslo u ucitalo samo what" << endl;
        }
        else if (args.size() == 2) {
            // Dvosmislen slučaj. Pravilo:
            // - ako postoji ulazna redirekcija (<), tumači kao WHAT, WITH (argument izostavljen)
            // - inače tumači kao ARGUMENT, WHAT (WITH izostavljen)
            if (isInputStream) {
                argForTr = "";        // ulaz dolazi iz <fajla
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
            // sve dato eksplicitno: ARGUMENT, WHAT, WITH
            argForTr = args[0];
            what = args[1];
            with = args[2];
        }

        return new Tr(command, argForTr, what, with);
    }
    else if (command == "time" || command == "date") {
        restOfLine >> ws;                 // preskoči praznine
        int ch = restOfLine.peek();

        if (ch == EOF) {
            // bez argumenata
        }
        else if (ch == '<' || ch == '>') {
            processRedirections(restOfLine, allowInputRedir, allowOutputRedir);
        }
        else {
            string junk;
            restOfLine >> junk;
            throw ArgumentException("Previše argumenata za komandu '" + command + "'.");
        }

        if (command == "time") return new Time(command);
        else                   return new Date(command);
    }
    else {
        throw CommandException("Nepoznata komanda.");
    }

    return nullptr;
}

// === KLJUČNA IZMJENA: ne “pojedaj” odvojene operatore; pusti processRedirections ===
void CommandFactory::readArgument(istringstream& stream, string& argument) {
    argument.clear();

    // preskoči whitespace
    stream >> ws;
    int ch1 = stream.peek();
    if (ch1 == EOF) return;

    if (ch1 == '<' || ch1 == '>') {
        // pogledaj da li je odvojeno ili spojeno
        // privremeno pročitaj prvi znak
        char c1; stream.get(c1);
        int ch2 = stream.peek();
        bool isAppendOp = (c1 == '>' && ch2 == '>');
        if (isAppendOp) {
            // pročitaj i drugi '>'
            char c2; stream.get(c2);
            ch2 = stream.peek();
        }

        // ako posle operatora sledi whitespace ili EOF -> ODVOJENO -> vrati operatore i izađi
        if (ch2 == EOF || isspace(ch2)) {
            // vrati šta smo pročitali
            stream.unget(); // vrati poslednji peek (nije potrošen, pa ako je EOF, ne radi unget)
            if (isAppendOp) stream.unget(); // vrati drugi '>'
            stream.unget(); // vrati prvi znak operatora
            return; // pusti processRedirections da odradi
        }
        else {
            // SPOJENO: vrati operatore pa pročitaj ceo token normalno
            if (isAppendOp) stream.unget();
            stream.unget();
            // sada normalno čitaj token u 'argument'
        }
    }

    // čitaj “normalan” argument/token (može biti i citiran)
    if (!(stream >> argument)) return;

    if (!argument.empty() && argument.front() == '"' && (argument.size() == 1 || argument.back() != '"')) {
        string tail; char c;
        while (stream.get(c)) {
            tail.push_back(c);
            if (c == '"') break;
        }
        argument += tail;
    }

    // Ako je spojena redirekcija (“<in.txt”, “>>out.txt”) – pojedi je ovde
    validateArgument(argument, true, true);
}

void CommandFactory::validateArgument(std::string& argument, bool allowInputRedir, bool allowOutputRedir) {
    if (argument.empty()) return;

    RedirTok rt;
    if (!parseRedirToken(argument, rt)) return;

    if (!rt.fname.empty()) {
        applyRedir(rt.kind, rt.fname, allowInputRedir, allowOutputRedir);
        argument.clear();
    }
}
