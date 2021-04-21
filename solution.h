#ifndef SOLUTION_H
#define SOLUTION_H
#include <iostream>
#include <vector>
#include <utility>

class solution{
        int task_num;
        int num;
        std::vector<std::pair<int, int> > bounds;
        int lower_est;
        int upper_est;
    public:
        solution(int tasknum);
        void get_lower_estimate();
        void get_upper_estimate();
        std::vector<std::pair<int, int> > get_bounds();
};
#endif