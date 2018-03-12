#include "grid_world_tabular_agent.h"
#include <iostream>

using namespace std;

void print_state(vector<vector<char>> state)
{
    std::cout << "size: " << state.size() << std::endl;
    for(int row = 0; row < state.size(); row++)
    {
        std::cout << "size2: " << state[0].size() << std::endl;
        for(int col = 0; col < state[0].size(); col++)
        {
            std::cout << state[row][col];
        }
        std::cout << std::endl;
    }
}

GridWorldTabularAgent::GridWorldTabularAgent(GridWorld env, float epsilon)
{
    this->numRows    = env.get_num_rows();
    this->numCols    = env.get_num_cols();
    this->numActions = env.get_num_actions();
    this->epsilon    = epsilon;

    std::random_device randomDevice;
    generator = std::mt19937(randomDevice());
    randomActionGenerator = std::uniform_int_distribution<>(0,env.get_num_actions()-1);
    rand = std::uniform_real_distribution<>(0,1);

    for(int row = 0; row < numRows; row++)
    {
        qValues.push_back(std::vector< std::vector<float> >());
        for(int col = 0; col < numCols; col++)
        {
            qValues[row].push_back(std::vector<float>());
            for(int action = 0; action < numActions; action++)
            {
                qValues[row][col].push_back(0.0f);
            }
        }
    }
}

void GridWorldTabularAgent::print_q_values()
{
    for(int row = 0; row < numRows; row++)
    {
        for(int col = 0; col < numCols; col++)
        {
            double max = 0;
            //std::cout << "(";
            for(int action = 0; action < numActions; action++)
            {
                //std::cout << qValues[row][col][action] << ",";
                if(qValues[row][col][action] > max)
                {
                    max = qValues[row][col][action];
                }
            }
            std::cout << max << " ";
            //std::cout << ") ";
        }
        std::cout << std::endl;
    }
}

int GridWorldTabularAgent::get_action(vector< vector<char> > board)
{
    if(rand(generator) <= epsilon)
    {
        return randomActionGenerator(generator);
    }
    return determine_best_action(board);
}

void GridWorldTabularAgent::process_transition(vector< vector<char> > state,
        int action, int reward, vector< vector<char> > nextState)
{
    Position position = find_agent(state);
    int row = position.row;
    int col = position.col;

    Position nextPosition = find_agent(nextState);
    int nextRow = nextPosition.row;
    int nextCol = nextPosition.col;
    
   // std::cout << "suspect 1" << std::endl;
    float Q = qValues[row][col][action];

    float learningRate = 0.01f;
    float discount     = 0.99f;

    int   bestAction = determine_best_action(nextState);
   // std::cout << "suspect 2" << std::endl;
    float QMax       = qValues[nextRow][nextCol][bestAction];

    Q = Q + learningRate*(reward+discount*QMax - Q);

    //std::cout << "suspect 3" << std::endl;
    qValues[row][col][action] = Q;
}

void GridWorldTabularAgent::log(Logger &logger, int time)
{
    for(int action = 0; action < numActions; action++)
    {
        std::string logKey = "Q_" + std::to_string(action);
        
        std::vector<float> qValues;
        for(int row = 0; row < numRows; row++)
        {
            for(int col = 0; col < numCols; col++)
            {
                qValues.push_back(this->qValues[row][col][action]);
            }
        }
        logger.get_log(logKey,"Time","Avg_Q").log(time,mean(qValues));
    }
}

/* We want to explore randomly during training (possibly anneal);
 * but follow the learned policy greedily during testing.
 */
void GridWorldTabularAgent::set_epsilon(float epsilon)
{
    this->epsilon = epsilon;
}

Position GridWorldTabularAgent::find_agent(vector< vector<char> > board)
{
    for(int row = 0; row < numRows; row++)
    {
        for(int col = 0; col < numCols; col++)
        {
            if(board[row][col] == GridWorld::AGENT)
            {
                return Position(row,col);
            }
        }
    }
    print_state(board);
    return Position(-1,-1);
}

int GridWorldTabularAgent::determine_best_action(vector< vector<char> > board)
{
    Position position = find_agent(board);
    int row = position.row;
    int col = position.col;

    int bestAction  = 0;
    float maxQValue = qValues[row][col][bestAction];
    for(int action = 0; action < numActions; action++)
    {
        if(qValues[row][col][action] > maxQValue)
        {
            maxQValue  = qValues[row][col][action];
            bestAction = action;
        }
    }
    return bestAction;

    /* Check in case there are multiple actions that are equally good */
    //std::vector<int> bestActions;
    //for(int action = 0; action < numActions; action++)
    //{
    //    if(float_equals(qValues[row][col][action],maxQValue))
    //    {
    //        bestActions.push_back(action);
    //    }
    //}

    /* Randomly sample from the vector of best actions */
    //std::uniform_int_distribution<> actionIndexPicker = 
    //        std::uniform_int_distribution<>(0,bestActions.size()-1);
    //return bestActions[actionIndexPicker(generator)];
}
