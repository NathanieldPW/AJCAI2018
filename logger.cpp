#include "logger.h"
#include <fstream>
#include <iostream>

Logger::Logger(std::string baseDirectory)
{
    this->baseDirectory = baseDirectory;
}

Log& Logger::get_log(std::string key, std::string xLabel, std::string yLabel)
{
    if(logs.find(key) == logs.end())
    {
        logs[key] = Log(xLabel,yLabel);
    }
    return logs[key];
}

void Logger::log_to_files(std::string parentDirectory)
{
    std::string directory = parentDirectory + "/" + baseDirectory;
    
    mkdir(directory);

    for(auto pair : logs)
    {
        std::string key = pair.first;
        Log log         = pair.second;

        std::string fileName = directory + "/" + key;
        std::ofstream logFile;
        logFile.open(fileName);

        logFile << "% " << log.get_xLabel() << " " << log.get_yLabel()
                << " " << log.get_yStddevLabel() << std::endl;

        std::map<int,Data> data = log.get_data();
        for(auto dataPair : data)
        {
            int  x     = dataPair.first;
            Data stats = dataPair.second;

            logFile << x << " " << stats.average << " " << stats.stddev
                    << std::endl;
        }
        logFile.close();
    }
}
