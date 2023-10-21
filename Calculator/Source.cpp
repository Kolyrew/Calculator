#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <windows.h>
#include <filesystem>

class PlugInterface {
public:
    virtual double Execute(const std::vector<double>& args) = 0;
    virtual std::string TakeFuncName() = 0;
};

class LoadPlugInSCPP {
private:
    std::vector<HMODULE> loadedPlugins;

    PlugInterface* loadIndividualExtension(const std::filesystem::path& filePath) {
        HMODULE hPlugin = LoadLibraryA(filePath.string().c_str());
        if (!hPlugin) return nullptr;

        using CreateFuncType = PlugInterface * (*)();
        CreateFuncType createFunc = reinterpret_cast<CreateFuncType>(GetProcAddress(hPlugin, "CreatePlugin"));

        if (!createFunc) {
            FreeLibrary(hPlugin);
            return nullptr;
        }

        loadedPlugins.push_back(hPlugin);
        return createFunc();
    }

public:
    std::vector<PlugInterface*> loadAllExtensionsFromDirectory(const std::string& folderPath) {
        std::vector<PlugInterface*> plugins;

        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            if (entry.path().extension() != ".dll") continue;

            PlugInterface* plugin = loadIndividualExtension(entry.path());
            if (plugin) {
                plugins.push_back(plugin);
            }
        }

        return plugins;
    }

    ~LoadPlugInSCPP() {
        for (HMODULE hPlugin : loadedPlugins) {
            FreeLibrary(hPlugin);
        }
    }
};

class Calculator { //считает основные и определяет нужный плагин

};

class ConsReader {
private:
    LoadPlugInSCPP loader;

public:
    double Evaluate(const std::string& operation, double arg1, double arg2) {
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

        auto plugins = loader.loadAllExtensionsFromDirectory("./plugins");
        for (auto plugin : plugins) {
            if (plugin->TakeFuncName() == operation) {
                double result = plugin->Execute({ arg1, arg2 });
                delete plugin;
                return result;
            }
            delete plugin;
        }

        throw std::runtime_error("Operation not found!");
    }
};


int main() {
    ConsReader evaluator;

    while (true) {
        std::cout << "Enter operation (+, -, *, /, custom function, or 'exit' to quit): ";
        std::string operation;
        std::cin >> operation;

        if (operation == "exit") {
            break;
        }

        std::cout << "Enter first argument: ";
        double arg1;
        std::cin >> arg1;

        std::cout << "Enter second argument: ";
        double arg2;
        std::cin >> arg2;

        try {
            double result = evaluator.Evaluate(operation, arg1, arg2);
            std::cout << "Result: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    return 0;
}