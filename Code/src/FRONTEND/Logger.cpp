#include "Logger.h"
#include <stdexcept>

Logger::Logger(const std::string& filename)
{
    logFile.open(filename, std::ios::out | std::ios::app);
    if (!logFile.is_open())
    {
        throw std::runtime_error("Could not open log file");
    }
}

Logger::~Logger()
{
    if (logFile.is_open())
    {
        logFile.close();
    }
}

void Logger::logOutput(const std::string& message)
{
    std::cout << message;
    if (logFile.is_open())
    {
        logFile << message;
    }
}

void Logger::logOutput(const char* message)
{
    std::cout << message;
    if (logFile.is_open())
    {
        logFile << message;
    }
}

void Logger::logInput(const std::string& input)
{
    std::string prompt = "> ";
    if (logFile.is_open())
    {
        logFile << prompt << input << std::endl;
    }
}