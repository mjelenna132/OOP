#pragma once
#pragma once
#include <stdexcept>
#include <string>

// Osnovna klasa za sve greške u komandama
class CommandException : public std::runtime_error {
public:
    explicit CommandException(const std::string& message)
        : std::runtime_error(message) {}
};

// Izuzetak za greške vezane za fajlove
class FileException : public CommandException {
public:
    explicit FileException(const std::string& message)
        : CommandException("Greška u fajlu: " + message) {}
};

// Izuzetak za neispravne argumente ili opcije
class ArgumentException : public CommandException {
public:
    explicit ArgumentException(const std::string& message)
        : CommandException("Greška u argumentu: " + message) {}
};

// Izuzetak za sintaksne greške
class SyntaxException : public CommandException {
public:
    explicit SyntaxException(const std::string& message)
        : CommandException("Sintaksna greška: " + message) {}
};