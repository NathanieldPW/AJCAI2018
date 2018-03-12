#include "grid_world.h"
#include "position.h"
#include "policy.h"
#include <vector>
#include <iostream>

const int NUM_ROWS = 5;
const int NUM_COLS = 5;

const char GridWorld::AGENT;
const char GridWorld::SPACE;
const char GridWorld::WALL;
const char GridWorld::GOAL;
const char GridWorld::HAZARD;

const char GridWorld::LEFT;
const char GridWorld::RIGHT;
const char GridWorld::UP;
const char GridWorld::DOWN;
const int  GridWorld::NUM_ACTIONS;
const char GridWorld::RANDOM;


void combos(std::vector< std::vector<Position> > &result, 
        std::vector<Position> in, unsigned int i)
{
    if(i >= in.size())
    {
        result.push_back(in);
        return;
    }

    /* Generate the combinations with the current wall included */
    combos(result,in,i+1);

    /* Generate the combinations without the current wall (in.size--) */
    in.erase(in.begin()+i);
    combos(result,in,i);
}

std::vector< std::vector<Position> > generate_all_wall_combinations(
        const std::vector<Position> &walls)
{
    std::vector< std::vector<Position> > result;

    /* Force at least one wall in the vector to be open */
   // for(unsigned int open = 0; open < walls.size(); open++)
   // {
        /* Make a copy of the walls vector so we can modify it */
    //    std::vector<Position> temp = walls;

        /* Remove the wall that we are forcing to be open */
      //  temp.erase(temp.begin()+open);

        /* Generate all possible combinations of on/off values for the
         * remaining walls, and add them to the result vector.
         */
        combos(result,walls,0);
        /* The first combo will have all walls filled in - impossible to pass */
        result.erase(result.begin());
 //   }
    return result;
}

template <class T>
bool contains(std::vector<T> list, T element)
{
    for(unsigned int i = 0; i < list.size(); i++)
    {
        if(list[i] == element)
        {
            return true;
        }
    }
    return false;
}

void add_mazes(std::vector<GridWorld>& mazes,
        int startRow, int startCol,
        int goalRow,  int goalCol)
{
    Position start( startRow, startCol );
    Position goal ( goalRow,  goalCol  );

    /* _W___
     * _W___
     * _W___
     * __WWW
     * _____
     */
    const std::vector<Position> walls1 = {
            Position(0,1),Position(1,1),Position(2,1),
            Position(3,2),Position(3,3),Position(3,4)};

    /* ___W_
     * ____W
     * _____
     * _____
     * _____
     */
    const std::vector<Position> walls2 = { Position(0,3), Position(1,4) };

    std::vector< std::vector<Position> > wall1Combos = 
            generate_all_wall_combinations(walls1);

    std::vector< std::vector<Position> > wall2Combos = 
            generate_all_wall_combinations(walls2);

    std::cout << "combos:" << wall1Combos.size() << " " << wall2Combos.size() << std::endl;

    for(unsigned int wall1Combo = 0; wall1Combo < wall1Combos.size(); wall1Combo++)
    {
        for(unsigned int wall2Combo = 0; wall2Combo < wall2Combos.size(); wall2Combo++)
        {
            GridWorld maze(NUM_ROWS,NUM_COLS,startRow,startCol);
            maze.place_walls(wall1Combos[wall1Combo]);
            maze.place_walls(wall2Combos[wall2Combo]);
            maze.place_goal(goalRow,goalCol);
            if(!contains(mazes,maze))
            {
                mazes.push_back(maze);
            }
        }
    }

}

int main(int arg, char* argv[])
{
    std::cout << "begin" << std::endl;

    std::vector<GridWorld> mazes;
 /*   for(int startRow = 0; startRow < NUM_ROWS; startRow++)
    {
        for(int startCol = 0; startCol < NUM_COLS; startCol++)
        {
            for(int goalRow = 0; goalRow < NUM_ROWS; goalRow++)
            {
                for(int goalCol = 0; goalCol < NUM_COLS; goalCol++)
                {
                    if(startRow == goalRow && startCol == goalCol)
                    {
                        continue;
                    }*/
                    add_mazes(mazes,0,0,0,1);//startRow,startCol,goalRow,goalCol);
                /*}
            }
        }
    }*/
    std::cout << "begin (again): " << mazes.size() << std::endl;
    std::vector<OptimalGridWorldPolicy> policies;
    for(auto maze : mazes)
    {
        policies.push_back(OptimalGridWorldPolicy(maze));
//        maze.print_board();
//        policies[policies.size()-1].print();
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, policies.size()-1);

    int sourceTaskIndex = dis(gen);
    std::priority_queue<GridWorld> queue;
    for(unsigned int index = 0; index < policies.size(); index++)
    {
        //double relevance = (double)1.0 / (policies[sourceTaskIndex] - policies[index]);
        double relevance = - (policies[sourceTaskIndex] - policies[index]);
        mazes[index].set_relevance(relevance);
        mazes[index].set_difficulty(policies[index].get_difficulty());
        queue.push(mazes[index]);
    }
    int i = 0;
    while(!queue.empty())
    {
        GridWorld maze = queue.top();
        std::cout << maze.get_relevance() << " " << maze.get_difficulty() << std::endl;
        std::cout << i++ << std::endl;
        maze.print_board();
        queue.pop();
    }
    //for(GridWorld maze : mazes)
    //{
    //    maze.print_board();
    //}
    std::cout << "end" << std::endl;
    return 0;
}
