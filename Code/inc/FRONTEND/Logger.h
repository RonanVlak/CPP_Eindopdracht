#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <iostream>
#include <string>

class Logger
{
public:
    static Logger& getInstance()
    {
        static Logger instance("session_log.txt");
        return instance;
    }

    void logOutput(const std::string& message);
    void logInput(std::string input);

private:
    Logger(const std::string& filename);
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::ofstream logFile;
};

#endif // LOGGER_H