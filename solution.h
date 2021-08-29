#ifndef SOLUTION_H
#define SOLUTION_H
#include <iostream>
#include <vector>
#include <utility>

class solution{
        int task_num;
        // std::vector<std::pair<int, int> > bounds;
        int lower_est;
        int upper_est;
    public:
        solution(int tasknum);
        int get_le();
        int get_ue();
        void get_lower_estimate(std::string path_file);
        void get_upper_estimate(std::string path_file);
        // std::vector<std::pair<int, int> > get_bounds();
};
#endif