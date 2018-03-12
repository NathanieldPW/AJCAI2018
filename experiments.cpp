#include "experiments.h"
#include "train.h"
#include "utils.h"
#include <ctime>

void deterministic_experiment()
{
    int numRows  = 1; int numCols  = 101;
    int startRow = 0; int startCol = 50;

    GridWorld nonCLEnv(numRows,numCols,startRow,startCol);
    nonCLEnv.place_goal(0,100);
    nonCLEnv.place_hazard(0,0);

    int leftStart = 1; int rightStart = 99;
    // Remove the two ends, and the middle, then halve
    int numIterations = (101-3)/2;

    std::vector<int> startCols;
    for(int i = 0; i < numIterations; i++)
    {
        startCols.push_back(rightStart--);
        startCols.push_back(leftStart++);
    }
    startCols.push_back(startCol);

    std::vector<GridWorld> envs;
    for(int colIndex = 0; colIndex < startCols.size(); colIndex++)
    {
        startCol = startCols[colIndex];
        envs.push_back(GridWorld(numRows,numCols,startRow,startCol));
        envs[colIndex].place_goal(0,100);
        envs[colIndex].place_hazard(0,0);
    }

    std::vector<int> difficultyBorders = {-1};
    for(int borderIndex = 1; borderIndex < startCols.size(); borderIndex+=2)
    {
        difficultyBorders.push_back(borderIndex);
    }
    difficultyBorders.push_back(envs.size()-1);

    const int TOTAL_GAMES      =   2000;
    const int TOTAL_ITERATIONS = 100000;

    std::time_t currentTime = time(NULL);
    char buffer[100];
    std:strftime(buffer,100,"%Y_%m_%d_%H:%M",std::localtime(&currentTime));
    std::string mainDirectory(buffer);
    mainDirectory += "_deterministic";
    std::string dataDirectory = mainDirectory + "/data";
    mkdir(mainDirectory);
    mkdir(dataDirectory);

    std::vector<GridWorld> nonCLEnvs = {nonCLEnv};
    std::vector<int> nonCLDB         = {-1,0};

    std::string experimentDirectory = dataDirectory + "/TvG";
    int numDataPoints = 20;
    run_experiment(nonCLEnvs,nonCLDB,envs,difficultyBorders,"Game",TOTAL_GAMES,numDataPoints,experimentDirectory);

    experimentDirectory = dataDirectory + "/QvT";
    numDataPoints = 30;
    run_experiment(nonCLEnvs,nonCLDB,envs,difficultyBorders,"Time",TOTAL_ITERATIONS,numDataPoints,experimentDirectory);
}

void deterministic_experiment_2D()
{
    int numRows  = 5; int numCols  = 5;
    int startRow = 4; int startCol = 0;

    /* _W_W_
     * _2_W_
     * _W__1
     * __WWW
     * 3____
     */
    const std::vector<Position> walls = {
            Position(0,1), Position(0,3),
            Position(1,3),
            Position(2,1),
            Position(3,2), Position(3,3), Position(3,4)
    };

    GridWorld nonCLEnv(numRows,numCols,startRow,startCol);
    nonCLEnv.place_goal(0,4);
    nonCLEnv.place_walls(walls);

    std::vector<GridWorld> envs = {
            GridWorld(numRows,numCols,2,4),
            GridWorld(numRows,numCols,1,1),
            GridWorld(numRows,numCols,4,0)
    };

    for(GridWorld& env : envs)
    {
        env.place_goal(0,4);
        env.place_walls(walls);
    }

    std::vector<int> difficultyBorders = {-1,0,1,2};

    const int TOTAL_GAMES      = 2000;
    const int TOTAL_ITERATIONS = 5000;

    std::time_t currentTime = time(NULL);
    char buffer[100];
    std:strftime(buffer,100,"%Y_%m_%d_%H:%M",std::localtime(&currentTime));
    std::string mainDirectory(buffer);
    mainDirectory += "_2D";
    std::string dataDirectory = mainDirectory + "/data";
    mkdir(mainDirectory);
    mkdir(dataDirectory);

    std::vector<GridWorld> nonCLEnvs = {nonCLEnv};
    std::vector<int> nonCLDB         = {-1,0};

    std::string experimentDirectory = dataDirectory + "/TvG";
    int numDataPoints = 20;
    run_experiment(nonCLEnvs,nonCLDB,envs,difficultyBorders,"Game",TOTAL_GAMES,numDataPoints,experimentDirectory);

    experimentDirectory = dataDirectory + "/RvT";
    numDataPoints = 30;
    run_experiment(nonCLEnvs,nonCLDB,envs,difficultyBorders,"Reward",TOTAL_ITERATIONS,numDataPoints,experimentDirectory);
}
