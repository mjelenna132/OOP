#pragma once
#include <stdexcept>
#include <string>

// Base class for all command-related exceptions
class CommandException : public std::runtime_error {
public:
    explicit CommandException(const std::string& message)
        : std::runtime_error(message) {}
};

// Exception for file-related errors
class FileException : public CommandException {
public:
    explicit FileException(const std::string& message)
        : CommandException("File error: " + message) {}
};

// Exception for invalid arguments or options
class ArgumentException : public CommandException {
public:
    explicit ArgumentException(const std::string& message)
        : CommandException("Argument error: " + message) {}
};

// Exception for syntax errors in commands
class SyntaxException : public CommandException {
public:
    explicit SyntaxException(const std::string& message)
        : CommandException("Syntax error: " + message) {}
};
