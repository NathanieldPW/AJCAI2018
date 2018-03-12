#include "train.h"


GridWorld generate_non_CL_env(const GridWorld& difficulty1)
{
    GridWorld nonCLEnv = difficulty1;
    // _#_#O
    // ___#_
    // _#___
    // __###
    // A____
    
    // _#_#O
    nonCLEnv.place_wall(0,1);
    nonCLEnv.place_wall(0,3);
    nonCLEnv.place_goal(0,4);

    // ___#_
    nonCLEnv.place_wall(1,3);

    // _#___
    nonCLEnv.place_wall(2,1);

    // __###
    nonCLEnv.place_wall(3,2);
    nonCLEnv.place_wall(3,3);
    nonCLEnv.place_wall(3,4);

    // A____
    // Already handled in constructor.

    GridWorld difficulty3 = nonCLEnv;
   
}

GridWorld generate_difficulty2(const GridWorld& nonCLEnv)
{
    // _#__O
    // _____
    // _#___
    // __###
    // A____
    GridWorld difficulty2 = nonCLEnv;
    difficulty2.place_space(0,3);
    difficulty2.place_space(1,3);
    return difficulty2;
}

void inter_difficulty_experiment()
{
    int numRows  = 5; int numCols  = 5;
    int startRow = 4; int startCol = 0;

    GridWorld difficulty1(numRows,numCols,startRow,startCol);
    GridWorld nonCLEnv    = generate_non_CL_env(difficulty1);
    GridWorld difficulty2 = generate_difficulty2(nonCLEnv);
    GridWorld difficulty3 = nonCLEnv
    
    std::vector<GridWorld> nonCLEnvs = {nonCLEnv};
    std::vector<int> nonCLDB         = {-1,0};

    std::vector<GridWorld> CLEnvs = {difficulty1, difficulty2, difficulty3};
    std::vector<int> difficultyBorders = {-1,0,1,2};

    run_experiment(nonCLEnvs,nonCLDB,nonCLEnvs,difficultyBorders,"Game",TOTAL_ITERATIONS,numDataPoints);
}
