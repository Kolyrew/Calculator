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
    double executeCalculation(const std::string& functionName, double argument) {
        auto plugins = loader.loadAllExtensionsFromDirectory("./plugins");
        for (auto plugin : plugins) {
            if (plugin->TakeFuncName() == functionName) {
                double result = plugin->Execute({ argument });
                delete plugin;
                return result;
            }
            delete plugin;
        }
        throw std::runtime_error("Function not found among plugins");
    }
};

int main() {
    ConsReader evaluator;
    std::string functionName;
    double arg;

    while (true) {
        std::cout << "Enter function name (or 'exit' to quit): ";
        std::cin >> functionName;

        if (functionName == "exit") {
            break;
        }

        std::cout << "Enter argument: ";
        std::cin >> arg;

        try {
            double result = evaluator.executeCalculation(functionName, arg);
            std::cout << "Result: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    return 0;
}