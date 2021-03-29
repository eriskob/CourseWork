#ifndef SOLUTION_H
#define SOLUTION_H
#include <iostream>
#include <vector>
#include <utility>

class solution{
        int num;
        std::vector<std::pair<int, int> > bounds;
    public:
        solution(int tasknum);
        std::vector<std::pair<int, int> > get_bounds();
};
#endif