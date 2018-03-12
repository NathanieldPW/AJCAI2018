#include "log.h"

Log::Log()
{
    xLabel = "Invalid Log Object";
    yLabel = "Invalid log Object";
}

Log::Log(std::string xLabel, std::string yLabel)
{
    this->xLabel = xLabel;
    this->yLabel = yLabel;
}

void Log::log(int x, float y)
{
    data[x].push_back(y);
}

std::string Log::get_xLabel()
{
    return xLabel;
}

std::string Log::get_yLabel()
{
    return yLabel;
}

std::string Log::get_yStddevLabel()
{
    return yLabel + "_stddev";
}

std::map<int,Data> Log::get_data()
{
    std::map<int,Data> result;
    for(auto pair : data)
    {
    int                x  = pair.first;
    std::vector<float> ys = pair.second;
    
    result[x]     = Data(mean(ys),stddev(ys));
    }
    return result;
}
