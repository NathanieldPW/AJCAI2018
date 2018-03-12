#include "train.h"

void subjective_difficulty_experiment()
{
    const int numRows  = 1; const int numCols  = 101;
    const int startRow = 0; const int startCol = 50;

    GridWorld nonCLEnv(numRows,numCols,startRow,startCol);
    nonCLEnv.place_goal(0,100);
    nonCLEnv.place_hazard(0,0);

    GridWorld nonCLEnv2(numRows,numCols,startRow,startCol);
    nonCLEnv2.place_goal(0,0); // The opposite environment
    nonCLEnv2.place_hazard(0,100);

    int leftStart = 1; int rightStart = 99;
    // Remove the two ends, and the middle, then halve
    int numIterations = (101-3)/2;

    std::vector<int> startCols;
    std::vector<GridWorld> envs;
    std::vector<int> difficultyBorders = {-1};

    
    /* Create the CL environments for the first experiemnt */


    for(int i = 0; i < numIterations; i++)
    {
        startCols.push_back(rightStart--);
        startCols.push_back(leftStart++);
    }
    startCols.push_back(startCol);

    for(int colIndex = 0; colIndex < startCols.size(); colIndex++)
    {
        int startCol = startCols[colIndex];
        envs.push_back(GridWorld(numRows,numCols,startRow,startCol));
        envs[colIndex].place_goal(0,100);
        envs[colIndex].place_hazard(0,0);
    }
    int borderIndex;
    for(borderIndex = 1; borderIndex < startCols.size(); borderIndex+=2)
    {
        difficultyBorders.push_back(borderIndex);
    }
    std::cout << borderIndex << std::endl;
    std::cout << envs.size()-1 << std::endl;;
    difficultyBorders.push_back(envs.size()-1);


    //print_envs(envs);

    /* Create the CL environments for the opposite experiment */


    for(int colIndex = 0; colIndex < startCols.size(); colIndex++)
    {
        int startCol = startCols[colIndex];
        GridWorld env(numRows,numCols,startRow,startCol);
        env.place_goal(0,0);
        env.place_hazard(0,100);
        envs.push_back(env);
    }

    leftStart = 1; rightStart = 99;
    for(int i = 0; i < numIterations; i++)
    {
        startCols.push_back(rightStart--);
        startCols.push_back(leftStart++);
    }
    startCols.push_back(startCol);

    for(borderIndex++; borderIndex < startCols.size(); borderIndex+=2)
    {
        difficultyBorders.push_back(borderIndex);
    }
    difficultyBorders.push_back(envs.size()-1);

    //print_envs(envs);



    const int TOTAL_GAMES      = 2*2000; // 10000
    const int TOTAL_ITERATIONS = 2*10000; // 100000

    std::time_t currentTime = time(NULL);
    char buffer[100];
    std:strftime(buffer,100,"%Y_%m_%d_%H:%M",std::localtime(&currentTime));
    std::string mainDirectory(buffer);
    mainDirectory += "_subjective_difficulty";
    std::string dataDirectory = mainDirectory + "/data";
    mkdir(mainDirectory);
    mkdir(dataDirectory);

    std::vector<GridWorld> nonCLEnvs = {nonCLEnv,nonCLEnv2};
    std::vector<int> nonCLDB         = {-1,0,1};

    std::string experimentDirectory = dataDirectory + "/TvG";
    int numDataPoints = 20;
    run_experiment(nonCLEnvs,nonCLDB,envs,difficultyBorders,"Game",TOTAL_GAMES,numDataPoints,experimentDirectory);

    experimentDirectory = dataDirectory + "/QvT";
    numDataPoints = 30;
    // pointless to repeat - already done in deterministic experiment
    //run_experiment(nonCLEnvs,nonCLDB,envs,difficultyBorders,"Time",TOTAL_ITERATIONS,numDataPoints,experimentDirectory);
}
