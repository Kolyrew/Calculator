#pragma once
#include <vector>
#include <string>

class PlugInterface {
public:
    virtual double Execute(const std::vector<double>& args) = 0;
    virtual std::string TakeFuncName() = 0;
    virtual ~PlugInterface() = default;
};
