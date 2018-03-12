#pragma once

#include "position.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <random>

class StateReward
{
    public:
        StateReward(std::vector< std::vector<char> > state, int reward)
        {
            this->state  = state;
            this->reward = reward;
        }
    public:
        std::vector< std::vector<char> > state;
        int reward;
};

#define RANDOM_ACTION '?'
class ProbActionPair
{
    public:
        ProbActionPair(float probability=1.0f, int action=RANDOM_ACTION)
        {
            this->probability = probability;
            this->action      = action;
        }
        bool operator==(const ProbActionPair &other) const
        {
            return float_equals(probability,other.probability) &&
                    action == other.action;
        }
        bool operator!=(const ProbActionPair &other) const
        {
            return !( (*this) == other );
        }
    public:
        float probability;
        int   action;
};

class GridWorld
{
    public:
        static const char AGENT;
        static const char SPACE;
        static const char WALL;
        static const char GOAL;
        static const char HAZARD;

        static const char LEFT;
        static const char RIGHT;
        static const char UP;
        static const char DOWN;
        static const int  NUM_ACTIONS;
        static const char RANDOM;

        GridWorld(int numRows, int numCols, int startRow, int startCol,
                float probability = 1);

        void place_goal(int row, int col);
        
        void place_wall(int row, int col);
        void place_wall(Position position);
        void place_walls(std::vector<Position> wallLocations);

        void place_hazard(int row, int col);
        void place_space(int row, int col);
        void set_probability(int row, int col, float probability,
                int action=RANDOM);
        bool game_over() const;
        std::vector< std::vector<char> > reset();
        StateReward step(int action);
        void print_board() const;
        bool in_bounds(int row, int col) const;
        int get_num_rows() const;
        int get_num_cols() const;
        int get_num_actions() const;
        Position get_agent_start_position() const;
        int get_space(int row, int col) const;
        int get_space(Position position) const;
        std::vector<Position> get_walkable_neighbours(int row, int col) const;
        std::vector<Position> get_walkable_neighbours(Position position) const;
        bool is_walkable(int space) const;
        bool is_walkable(int row, int col) const;
        double get_relevance();
        void set_relevance(double relevance);
        double get_difficulty();
        void set_difficulty(double difficulty);
        /* For sorting in a priority queue */
        bool operator<(const GridWorld &other) const;
        bool operator==(const GridWorld &other) const;
    private:
        void add_walkable_neighbour(std::vector<Position> &neighbours, int row, int col) const;
        void initialise_boards(float probability);
        void place_tile(int row, int col, char tile);
        int determine_action(int action);
    private:
        int numRows;
        int numCols;
        int agentRow;
        int agentCol;
        int agentStartRow;
        int agentStartCol;
        bool gameOver;
        double relevance;
        double difficulty;

        std::vector< std::vector<char> > board;
        std::vector< std::vector<ProbActionPair> > probabilities;

        std::mt19937 generator;
        std::uniform_int_distribution<> randomActionGenerator;
        std::uniform_real_distribution<> rand;

};
