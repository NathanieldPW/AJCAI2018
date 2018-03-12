#include "utils.h"
#include <cmath>

bool float_equals(float a, float b)
{
    return (a-b) < 1e-10;
}

float sum(std::vector<float> values)
{
    float result = 0;
    for(float value : values)
    {
        result += value;
    }
    return result;
}

float mean(std::vector<float> values)
{
    return sum(values) / (float)values.size();
}

float stddev(std::vector<float> values)
{
    float average = mean(values);
    float variance = 0;
    for(float value : values)
    {
        variance += pow(value-average,2);
    }
    variance /= (values.size()-1);
    return sqrt(variance);
}

void mkdir(std::string directory)
{
    std::string systemCommand = "mkdir " + directory;
    system(systemCommand.c_str());
}
