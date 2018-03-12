#pragma once

#include "grid_world.h"
#include "position.h"
#include "logger.h"
#include <random>
#include <vector>

class GridWorldTabularAgent
{
public:
    GridWorldTabularAgent(GridWorld env, float epsilon=1.0f);
    
    void print_q_values();
    
    int get_action(std::vector< std::vector<char> > board);

    void process_transition(std::vector< std::vector<char> > state, int action,
            int reward, std::vector< std::vector<char> > nextState);
    
    void log(Logger &logger, int time);

    /* We want to explore randomly during training (possibly anneal);
     * but follow the learned policy greedily during testing.
     */
    void set_epsilon(float epsilon);

private:
    Position find_agent(std::vector< std::vector<char> > board);
    int determine_best_action(std::vector< std::vector<char> > board);
private:
    int numRows;
    int numCols;
    int numActions;
    float epsilon;
    std::vector< std::vector< std::vector<float> > > qValues;

    std::mt19937 generator;
    std::uniform_int_distribution<>  randomActionGenerator;
    std::uniform_real_distribution<> rand;
};
