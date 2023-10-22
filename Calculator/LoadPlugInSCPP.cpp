#include "LoadPlugInSCPP.h"

PlugInterface* LoadPlugInSCPP::loadIndividualExtension(const std::filesystem::path& filePath) {
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

std::vector<PlugInterface*> LoadPlugInSCPP::loadAllExtensionsFromDirectory(const std::string& folderPath) {
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

LoadPlugInSCPP::~LoadPlugInSCPP() {
    for (HMODULE hPlugin : loadedPlugins) {
        FreeLibrary(hPlugin);
    }
}
