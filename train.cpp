#include "train.h"
#include "grid_world_tabular_agent.h"
#include "logger.h"
#include "counter.h"
#include <ctime>
#include <iostream>

#define DEBUG 0

static int dump = 0;
int train_in_env(GridWorld& env, GridWorldTabularAgent& agent,
        std::string logMode, int countsPerEnv, int& totalReward = dump)
{
    std::vector< std::vector<char> > state = env.reset();

    Counter  gameCounter(0,true);
    Counter  timeCounter(0,false,countsPerEnv);
    Counter* mainCounter = &timeCounter;
    if(logMode == "Game")
    {
        timeCounter = Counter(0,true);
        gameCounter = Counter(0,false,countsPerEnv);
        mainCounter = &gameCounter;
    }

    while(!mainCounter->limit_reached())
    {
        int action = agent.get_action(state);


        #if DEBUG
        std::cout << "---------------" << std::endl;
        env.print_board();
        std::cout << action << std::endl;
        #endif

        StateReward stateReward = env.step(action);
        std::vector< std::vector<char> > newState = stateReward.state;
        int reward = stateReward.reward;
        totalReward += reward;

        agent.process_transition(state,action,reward,newState);

        state = newState;

        if(env.game_over())
        {
            state = env.reset();
            gameCounter.inc();
        }
        timeCounter.inc();
    }
    return timeCounter.get_value();
}

int test_agent(GridWorld env, GridWorldTabularAgent agent)
{
    /* Follow the learned policy during testing (no randomness). */
    agent.set_epsilon(0);
    int totalReward = 0;

    /* It takes 10 steps for the optimal 2D agent to reach its goal.
     * With a 100 step time limit, the optimal total reward is
     * 10 games * 10 (reward value) = 100
     *
     * The 1D game takes 50 steps optimally => max total reward of 20.
     */
    train_in_env(env, agent, "Reward", 100, totalReward);
    
    return totalReward;
}

void run_trial(const std::vector<GridWorld>& envs,
        const std::vector<int>& difficultyBorders,
        int count, std::string logMode, Logger& logger)
{
    GridWorldTabularAgent agent(envs[0]);

    int countsPerDifficulty = count / (difficultyBorders.size()-1);
    int totalTrainingTime   = 0;

    for(int difficulty = 1; difficulty < difficultyBorders.size(); difficulty++)
    {
        int envIndexMin = difficultyBorders[difficulty-1]+1;
        int envIndexMax = difficultyBorders[difficulty];

        int numEnvsInDifficulty = envIndexMax - envIndexMin + 1;
        int countsPerEnv        = countsPerDifficulty / numEnvsInDifficulty;


        for(int envIndex = envIndexMin; envIndex <= envIndexMax; envIndex++)
        {
            GridWorld env = envs[envIndex];
            int time = train_in_env(env,agent,logMode,countsPerEnv);
            totalTrainingTime += time;
        }
    }
    if(logMode == "Game")
    {
        int numGames = count;
        logger.get_log("TvG","#Games","Time").log(numGames,totalTrainingTime);
    }
    if(logMode == "Time")
    {
        int time = count;
        agent.log(logger,time);
    }
    if(logMode == "Reward")
    {
        /* Test on the hardest environment */
        GridWorld env = envs[envs.size()-1];
        int testReward = test_agent(env,agent);
        logger.get_log("RvT","Time","Reward").log(totalTrainingTime,testReward);
    }
    //std::cout << "---------------" << std::endl;
    //agent.print_q_values();
}

void print_envs(const std::vector<GridWorld>& envs)
{
    std::cout << "------------------------------------------+++++++++++++++" << std::endl;
    for(auto env : envs)
    {
        env.print_board();
    }
    std::cout << "------------------------------------------+++++++++++++++" << std::endl;
}

void run_experiment(std::vector<GridWorld>& nonCLEnvs,
        std::vector<int>& nonCLDB,
        std::vector<GridWorld>& envs,
        std::vector<int>& difficultyBorders, std::string logMode,
        const int TIME_LIMIT,
        const int NUM_DATA_POINTS, std::string experimentDirectory)
{
    const int LOG_INTERVAL = TIME_LIMIT / NUM_DATA_POINTS;
    const int NUM_TRIALS   = 200;
    std::cout << std::fixed;
    std::cout.precision(3);
    


    Logger nonCLLogger("nonCL");
    for(int trial = 0; trial < NUM_TRIALS; trial++)
    {
        std::cout << "trial (nonCL) = " << trial << std::endl;
        for(int time = 0; time <= TIME_LIMIT; time += LOG_INTERVAL)
        {
            run_trial(nonCLEnvs,nonCLDB,time,logMode,nonCLLogger);
        }
    }

    Logger CLLogger("CL");
    for(int trial = 0; trial < NUM_TRIALS; trial++)
    {
        std::cout << "trial (CL) = " << trial << std::endl;
        for(int time = 0; time <= TIME_LIMIT; time+=LOG_INTERVAL)
        {
            run_trial(envs,difficultyBorders,time,logMode,CLLogger);
        }
    }

    // mkdir experimentDirectory
    mkdir(experimentDirectory);
    nonCLLogger.log_to_files(experimentDirectory);
    CLLogger.log_to_files(experimentDirectory);
}
