#include "train.h"

void soft_barrier_experiment()
{
    int numRows  = 1; int numCols  = 101;
    int startRow = 0; int startCol = 50;

    GridWorld nonCLEnv(numRows,numCols,startRow,startCol);
    nonCLEnv.place_goal(0,100);
    nonCLEnv.place_goal(0,0);
    /* Give the location next to the left-most goal a small probability
     * of letting the agent action go through and (thus) a large probability
     * of forcing the agent to go right instead.
     */
    nonCLEnv.set_probability(0,1,0.001,GridWorld::RIGHT);

    std::vector<int> startCols;
    for(int startCol = 99; startCol >= 1; startCol--)
    {
        startCols.push_back(startCol);
    }

    std::vector<GridWorld> envs;
    for(int colIndex = 0; colIndex < startCols.size(); colIndex++)
    {
        startCol = startCols[colIndex];
        envs.push_back(GridWorld(numRows,numCols,startRow,startCol));
        envs[colIndex].place_goal(0,100);
        envs[colIndex].place_goal(0,0);
        envs[colIndex].set_probability(0,1,0.001,GridWorld::RIGHT);
    }

    std::vector<int> difficultyBorders = {-1};
    for(int borderIndex = 1; borderIndex < startCols.size(); borderIndex++)
    {
        difficultyBorders.push_back(borderIndex);
    }
    //difficultyBorders.push_back(envs.size()-1); // ???

    const int TOTAL_GAMES      = 1000; // 10000
    const int TOTAL_ITERATIONS = 10000; // 100000

    std::time_t currentTime = time(NULL);
    char buffer[100];
    std:strftime(buffer,100,"%Y_%m_%d_%H:%M",std::localtime(&currentTime));
    std::string mainDirectory(buffer);
    mainDirectory += "_soft_barrier";
    std::string dataDirectory = mainDirectory + "/data";
    mkdir(mainDirectory);
    mkdir(dataDirectory);

    std::vector<GridWorld> nonCLEnvs = {nonCLEnv};//envs[envs.size()-1]};
    std::vector<int> nonCLDB         = {-1,0};

    std::string experimentDirectory = dataDirectory + "/TvG";
    int numDataPoints = 20;
    run_experiment(nonCLEnvs,nonCLDB,envs,difficultyBorders,"Game",TOTAL_GAMES,numDataPoints,experimentDirectory);

    experimentDirectory = dataDirectory + "/QvT";
    numDataPoints = 30;
    run_experiment(nonCLEnvs,nonCLDB,envs,difficultyBorders,"Time",TOTAL_ITERATIONS,numDataPoints,experimentDirectory);
}
