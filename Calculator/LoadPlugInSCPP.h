#pragma once
#include <vector>
#include <windows.h>
#include <filesystem>
#include "PlugInterface.h"

class LoadPlugInSCPP {
private:
    std::vector<HMODULE> loadedPlugins;
    PlugInterface* loadIndividualExtension(const std::filesystem::path& filePath);

public:
    std::vector<PlugInterface*> loadAllExtensionsFromDirectory(const std::string& folderPath);
    ~LoadPlugInSCPP();
};
