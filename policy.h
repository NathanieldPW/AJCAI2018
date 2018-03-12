#pragma once

#include "grid_world.h"
#include <queue>

class Policy
{
    public:
        Policy(){}
        Policy(int numRows, int numCols)
        {
            init(numRows,numCols);
        }
        int get_action(int row, int col)
        {
            if(0 <= row && row < numRows && 0 <= col && col < numCols)
            {
                return actions[row][col];
            }
            return GridWorld::RANDOM;
        }
        void set_action(int row, int col, int action)
        {
            if(0 <= row && row < numRows && 0 <= col && col < numCols)
            {
                actions[row][col] = action;
            }
        }
    protected:
        void init(int numRows, int numCols)
        {
            this->numRows = numRows;
            this->numCols = numCols;
            for(int row = 0; row < numRows; row++)
            {
                actions.push_back(std::vector<char>());
                for(int col = 0; col < numCols; col++)
                {
                    actions[row].push_back(GridWorld::RANDOM);
                }
            }
        }
    protected:
        int numRows;
        int numCols;
        std::vector< std::vector<char> > actions;
};

class OptimalGridWorldPolicy : public Policy
{
    public:
        OptimalGridWorldPolicy(const GridWorld &maze) : Policy(maze.get_num_rows(), maze.get_num_cols())
        {
            djikstras(maze);
        }
        void print()
        {
            for(int row = 0; row < numRows; row++)
            {
                for(int col = 0; col < numCols; col++)
                {
                    std::cout << actions[row][col];
                }
                std::cout << std::endl;
            }
        }
        double operator-(const OptimalGridWorldPolicy &other) const
        {
            static const double epsilon = 1e-12;
            double difference = 0.0;
            for(int row = 0; row < numRows; row++)
            {
                for(int col = 0; col < numCols; col++)
                {
                    if(actions[row][col] != other.actions[row][col])
                    {
                        /* For a deterministic-ish policy, we will differ at
                         * exactly two actions - the rest of the actions will
                         * have zero-ish probability in both policies.
                         */

                        /* For the first action, one policy thinks the
                         * probability is zero-ish, and the other thinks it
                         * is one-ish.
                         */
                        double P = epsilon / (double)(GridWorld::NUM_ACTIONS-1);
                        double Q = 1-epsilon;

                        /* KLDiv = sum_i -(P(a) * log(P(a) / Q(a))) */
                        difference += -( P* log(Q/P) );

                        /* Swap P and Q for the second action, and go again */
                        P = 1-epsilon;
                        Q = epsilon / (double)(GridWorld::NUM_ACTIONS-1);
                        difference += -( P* log(Q/P) );
                    }
                    /* else, no difference for this position */
                }
            }
            return difference;
        }
        int get_difficulty()
        {
            return difficulty;
        }
    private:
        Position find_goal(const GridWorld &maze)
        {
            for(int row = 0; row < numRows; row++)
            {
                for(int col = 0; col < numCols; col++)
                {
                    if(maze.get_space(row,col) == GridWorld::GOAL)
                    {
                        return Position(row,col);
                    }
                }
            }
            return Position(-1,-1);
        }
        class Node
        {
            public:
                Node(Position position, int priority): position(position), priority(priority)
                {
                 //   this->position = position;
                 //   this->priority = priority;
                }
                bool operator<(const Node &other) const
                {
                    return priority > other.priority; // *min* priority queue
                }
            public:
                Position position;
                int priority;
        };
        void determine_action(int row, int col, int nextRow, int nextCol)
        {
            /* if(we need to move to the right) */
            if(col < nextCol)
            {
                actions[row][col] = GridWorld::RIGHT;
            }
            /* if(we need to move to the left) */
            if(col > nextCol)
            {
                actions[row][col] = GridWorld::LEFT;
            }
            /* if(we need to move down (topleft = (0,0)) ) */
            if(row < nextRow)
            {
                actions[row][col] = GridWorld::DOWN;
            }
            /* if(we need to move up (topleft = (0,0)) ) */
            if(row > nextRow)
            {
                actions[row][col] = GridWorld::UP;
            }
        }
        void djikstras(const GridWorld &maze)
        {
            Position goalPosition = find_goal(maze);
            //std::cout << "Goal found at: (" << goalPosition.row << "," << goalPosition.col << ")" << std::endl;

            std::vector< std::vector<int> > distances;

            std::priority_queue<Node> queue;
            for(int row = 0; row < numRows; row++)
            {
                distances.push_back(std::vector<int>());
                for(int col = 0; col < numCols; col++)
                {
                    distances[row].push_back(1000000);
                    if(maze.is_walkable(row,col))
                    {
                        queue.push(Node(Position(row,col),distances[row][col]));
                    }
                }
            }
            queue.push(Node(goalPosition,0));
            distances[goalPosition.row][goalPosition.col] = 0;
            //std::cout << "TOP: " << queue.top().priority << std::endl;
            while(!queue.empty())
            {
                Node currentNode = queue.top();
                queue.pop();

                Position current = currentNode.position;
                /* We have no update method for prioirties.
                 * So, things are added multiple times, and ignored if
                 * examined with a stale priority.
                 * All nodes should have a priority equal to their distance
                 * */
                if(currentNode.priority != distances[current.row][current.col])
                {
                    continue;
                }

                std::vector<Position> neighbours = maze.get_walkable_neighbours(current);
                for(Position neighbour : neighbours)
                {
                    int possibleDistance = distances[current.row][current.col]+1;
                    if(possibleDistance < distances[neighbour.row][neighbour.col])
                    {
                        distances[neighbour.row][neighbour.col] = possibleDistance;
                        queue.push(Node(neighbour,possibleDistance));
                        /* We want the neighbour to know how to get to the current position */
                        determine_action(neighbour.row,neighbour.col,current.row,current.col);
                    }
                }
            }
            Position agentPosition = maze.get_agent_start_position();
            difficulty = distances[agentPosition.row][agentPosition.col];
        }

    private:
        int difficulty;
};
