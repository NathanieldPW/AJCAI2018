#pragma once

#include "grid_world.h"
#include <vector>
#include <string>

void print_envs(const std::vector<GridWorld>& envs);

void run_experiment(std::vector<GridWorld>& nonCLEnvs,
        std::vector<int>& nonCLDB,
        std::vector<GridWorld>& envs,
        std::vector<int>& difficultyBorders,
        std::string logMode,
        const int TIME_LIMIT,
        const int NUM_DATA_POINTS, std::string experimentDirectory);
