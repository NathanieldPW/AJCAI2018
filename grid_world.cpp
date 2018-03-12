#include "grid_world.h"
#include "position.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <random>

const char GridWorld::AGENT  = 'A';
const char GridWorld::SPACE  = '_';
const char GridWorld::WALL   = '#';
const char GridWorld::GOAL   = 'O';
const char GridWorld::HAZARD = 'X';

const char GridWorld::LEFT        = 0; //'L';
const char GridWorld::RIGHT       = 1; //'R';
const char GridWorld::UP          = 2; //'U';
const char GridWorld::DOWN        = 3; //'D';
const int  GridWorld::NUM_ACTIONS = 4;
const char GridWorld::RANDOM      = RANDOM_ACTION;

 GridWorld::GridWorld(int numRows, int numCols, int startRow, int startCol,
        float probability)
{
    this->numRows       = numRows;
    this->numCols       = numCols;
    this->agentStartRow = startRow;
    this->agentStartCol = startCol;

    std::random_device randomDevice;            
    generator = std::mt19937(randomDevice());
    randomActionGenerator = std::uniform_int_distribution<>(0,get_num_actions()-1);
    rand = std::uniform_real_distribution<>(0,1);

    initialise_boards(probability);
    reset();
}

void GridWorld::place_goal(int row, int col)
{
    place_tile(row,col,GOAL);
}

void GridWorld::place_wall(int row, int col)
{
    /* You can't place a wall on the spot where the agent starts */
    if(row != agentStartRow  || col != agentStartCol)
    {
        place_tile(row,col,WALL);
    }
}

void GridWorld::place_wall(Position position)
{
    place_wall(position.row,position.col);
}

void GridWorld::place_walls(std::vector<Position> wallLocations)
{
    for(Position position : wallLocations)
    {
        place_wall(position);
    }
}

void GridWorld::place_hazard(int row, int col)
{
    place_tile(row,col,HAZARD);
}

void GridWorld::place_space(int row, int col)
{
    place_tile(row,col,SPACE);
}

void GridWorld::set_probability(int row, int col, float probability,
        int action)
{
    probabilities[row][col] = ProbActionPair(probability,action);
}

bool GridWorld::game_over() const
{
    return gameOver;
}

std::vector< std::vector<char> > GridWorld::reset()
{
    gameOver = false;
    agentRow = agentStartRow;
    agentCol = agentStartCol;
    board[agentRow][agentCol] = AGENT;
    return board;
}

StateReward GridWorld::step(int action)
{
    int newRow = agentRow;
    int newCol = agentCol;
    action = determine_action(action);

    switch(action)
    {
    case UP:
        newRow--;
        break;
    case DOWN:
        newRow++;
        break;
    case LEFT:
        newCol--;
        break;
    case RIGHT:
        newCol++;
        break;
    }

    int reward = 0;

    if(in_bounds(newRow,newCol) && board[newRow][newCol] != WALL)
    {
        board[agentRow][agentCol] = SPACE;
        
        agentRow = newRow;
        agentCol = newCol;

        if(board[agentRow][agentCol] == GOAL)
            reward =  10;
        if(board[agentRow][agentCol] == HAZARD)
            reward = -10;

        if(reward != 0)
        {
            gameOver = true;
            std::vector< std::vector<char> > resultBoard = board;
            resultBoard[agentRow][agentCol] = AGENT;
            return StateReward(resultBoard,reward);
        }
        board[agentRow][agentCol] = AGENT;
    }
    return StateReward(board,reward);
}

void GridWorld::print_board() const
{
    for(int row = 0; row < numRows; row++)
    {
        for(int col = 0; col < numCols; col++)
        {
            std::cout << board[row][col];
        }
        std::cout << std::endl;
    }
}

bool GridWorld::in_bounds(int row, int col) const
{
    return 0 <= row && row < numRows && 0 <= col && col < numCols;
}

int GridWorld::get_num_rows() const
{
    return numRows;
}

int GridWorld::get_num_cols() const
{
    return numCols;
}

int GridWorld::get_num_actions() const
{
    if(numRows == 1)
    {
        return 2;       // 1D
    }
    return NUM_ACTIONS; // 2D
}

Position GridWorld::get_agent_start_position() const
{
    return Position(agentStartRow,agentStartCol);
}

int GridWorld::get_space(int row, int col) const
{
    if(in_bounds(row,col))
    {
        return board[row][col];
    }
    return -1;
}

int GridWorld::get_space(Position position) const
{
    return get_space( position.row, position.col );
}

std::vector<Position> GridWorld::get_walkable_neighbours(int row, int col) const
{
    std::vector<Position> neighbours;

    add_walkable_neighbour( neighbours, row,   col+1 );
    add_walkable_neighbour( neighbours, row,   col-1 );
    add_walkable_neighbour( neighbours, row+1, col   );
    add_walkable_neighbour( neighbours, row-1, col   );

    return neighbours;
}

std::vector<Position> GridWorld::get_walkable_neighbours(Position position) const
{
    return get_walkable_neighbours(position.row,position.col);
}

bool GridWorld::is_walkable(int space) const
{
    return space != WALL;
}

bool GridWorld::is_walkable(int row, int col) const
{
    return is_walkable(get_space(row,col));
}

double GridWorld::get_relevance()
{
    return relevance;
}

void GridWorld::set_relevance(double relevance)
{
    this->relevance = relevance;
}

double GridWorld::get_difficulty()
{
    return difficulty;
}

void GridWorld::set_difficulty(double difficulty)
{
    this->difficulty = difficulty;
}

/* For sorting in a priority queue */
bool GridWorld::operator<(const GridWorld &other) const
{
    return relevance < other.relevance;
}

bool GridWorld::operator==(const GridWorld &other) const
{
    if(numRows       != other.numRows       ||
       numCols       != other.numCols       ||
       agentRow      != other.agentRow      ||
       agentCol      != other.agentCol      ||
       agentStartRow != other.agentStartRow ||
       agentStartCol != other.agentStartCol ||
       gameOver      != other.gameOver)
    {
        return false;
    }

    // These are derived quantities.
    //
    //double relevance;
    //double difficulty;

    for(int row = 0; row < numRows; row++)
    {
        for(int col = 0; col < numCols; col++)
        {
            if(board[row][col] != other.board[row][col])
            {
                return false;
            }
            if(probabilities[row][col] != other.probabilities[row][col])
            {
                return false;
            }
        }
    }
    /* At this point, everything must be equal */
    return true;
}

void GridWorld::add_walkable_neighbour(std::vector<Position> &neighbours, int row, int col) const
{
    if(in_bounds(row,col) && is_walkable(board[row][col]))
    {
        neighbours.push_back(Position(row,col));
    }
}

void GridWorld::initialise_boards(float probability)
{
    board.clear();
    probabilities.clear();
    for(int row = 0; row < numRows; row++)
    {
        board.push_back(std::vector<char>());
        probabilities.push_back(std::vector<ProbActionPair>());
        for(int col = 0; col < numCols; col++)
        {
            board[row].push_back(SPACE);
            probabilities[row].push_back(ProbActionPair(probability));
        }
    }
}

void GridWorld::place_tile(int row, int col, char tile)
{
    if(in_bounds(row,col))
    {
        board[row][col] = tile;
    }
}

int GridWorld::determine_action(int action)
{
    /* if(the selected action goes through) */
    if(rand(generator) <= probabilities[agentRow][agentCol].probability)
    {
        return action;
    }

    /* At this point the environment is stochastic and has rejected
     * the agent's desired action. We return the environment action
     * instead - which requires a little work if the environment action
     * is set to random.
     */
    if(probabilities[agentRow][agentCol].action == RANDOM)
    {
        return randomActionGenerator(generator);
    }
    return probabilities[agentRow][agentCol].action;
}
