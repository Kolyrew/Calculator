#pragma once
#include <string>
#include <vector>
#include "LoadPlugInSCPP.h"

class ConsReader {
private:
    LoadPlugInSCPP loader;
    void ParseInput(const std::string& input, std::string& operation, std::vector<double>& args);
    bool IsStandardOperation(const std::string& operation);
    double EvaluateStandardOperation(const std::string& operation, double arg1, double arg2);

public:
    double Evaluate(const std::string& input);
};
