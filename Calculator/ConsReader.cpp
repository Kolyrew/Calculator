#include "ConsReader.h"
#include <sstream>
#include <stdexcept>
#include <cctype>

void ConsReader::ParseInput(const std::string& input, std::string& operation, std::vector<double>& args) {
    std::istringstream iss(input);
    std::string token;

    while (iss >> token) {
        if (std::isdigit(token[0]) || (token[0] == '-' && token.size() > 1)) {
            args.push_back(std::stod(token));
        }
        else if (operation.empty()) {
            operation = token;
        }
        else {
            throw std::runtime_error("Invalid input format!");
        }
    }

    if (operation.empty() && args.size() == 1) {
        operation = "id"; // Identity operation, returns the argument as is
    }
}

bool ConsReader::IsStandardOperation(const std::string& operation) {
    return operation == "+" || operation == "-" || operation == "*" || operation == "/";
}

double ConsReader::EvaluateStandardOperation(const std::string& operation, double arg1, double arg2) {
    if (operation == "+") {
        return arg1 + arg2;
    }
    else if (operation == "-") {
        return arg1 - arg2;
    }
    else if (operation == "*") {
        return arg1 * arg2;
    }
    else if (operation == "/") {
        if (arg2 == 0) {
            throw std::runtime_error("Division by zero!");
        }
        return arg1 / arg2;
    }
    throw std::runtime_error("Invalid standard operation!");
}

double ConsReader::Evaluate(const std::string& input) {
    std::string operation;
    std::vector<double> args;
    ParseInput(input, operation, args);

    if (operation.empty()) {
        throw std::runtime_error("Invalid input!");
    }

    if (IsStandardOperation(operation)) {
        if (args.size() != 2) {
            throw std::runtime_error("Invalid number of arguments for standard operation!");
        }
        return EvaluateStandardOperation(operation, args[0], args[1]);
    }
    else {
        auto plugins = loader.loadAllExtensionsFromDirectory("./plugins");
        for (auto plugin : plugins) {
            if (plugin->TakeFuncName() == operation) {
                double result = plugin->Execute(args);
                delete plugin;
                return result;
            }
            delete plugin;
        }
    }

    throw std::runtime_error("Operation not found!");
}
