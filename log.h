#pragma once

#include "utils.h"
#include <string>
#include <vector>
#include <map>

class Data 
{
    public:
        Data()
        {
            average = -1;
            stddev  = -1;
        }
        Data(float average, float stddev)
        {
            this->average = average;
            this->stddev = stddev;
        }
    public:
        float average;
        float stddev;
};

class Log
{
    public:
        Log();
        Log(std::string xLabel, std::string yLabel);
        void log(int x, float y);
        std::string get_xLabel();
        std::string get_yLabel();
        std::string get_yStddevLabel();
        std::map<int,Data> get_data();
    private:
        std::string xLabel;
        std::string yLabel;
        std::map< int,std::vector<float> > data;
};
