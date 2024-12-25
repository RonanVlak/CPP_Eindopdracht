#pragma once

#include <string>
#include <filesystem>
#ifdef _WIN32
#include <windows.h>
#define PATH_MAX MAX_PATH
#else
#include <unistd.h>
#include <limits.h>
#endif
#include <stdexcept>

class FSConverter {
public:
    FSConverter(std::string ResourceDir = "");

    std::string getResourcePath(const std::string& resourceName);

private:
    std::string findResourcesFolder();
    std::string executablePath();
    std::string resourceDir;
};