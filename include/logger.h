#ifndef LOGGER_H
#define LOGGER_H
#include <ctime>
#include <fstream>
#include <sstream>
#include <iostream>
#include <mutex>
using namespace std;

class Logger
{
    std::ofstream logFile;

public:
    explicit Logger(const string &filename)
    {

        logFile.open(filename, ios::app);

        if (!logFile.is_open())
        {
            std::cerr << " Error opening file!\n";
        }
    }

    ~Logger()
    {
        logFile.close();
    }

    void log(const string &message)
    {
        std::ostringstream logEntry;
        logEntry << message << "\n";

        if (logFile.is_open())
        {
            logFile << logEntry.str();
            logFile.flush();
        }
    }
};

#endif