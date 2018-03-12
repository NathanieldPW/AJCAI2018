#pragma once

#include "log.h"
#include <fstream>
#include <iostream>

class Logger
{
    public:
        Logger(std::string baseDirectory);
        Log& get_log(std::string key, std::string xLabel, std::string yLabel);
        void log_to_files(std::string parentDirectory);
    private:
        std::map<std::string,Log> logs;
        std::string baseDirectory;
};
