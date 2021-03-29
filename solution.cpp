#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include "task.h"
#include "solution.h"

solution::solution(int tasknum){
    task target(tasknum);
    std::vector<task> anomal_tasks;
    target.find_anomaltasks();
    this->num = target.get_anomaltasks().size();
    for(int i = 0; i < num; i++){
        anomal_tasks.push_back(target.get_anomaltasks()[i]);
    }
    for(int i = 0; i < num; i++){
        bounds.push_back(std::pair<int, int>());
        bounds[i].first = anomal_tasks[i].get_bcet();
        bounds[i].second = anomal_tasks[i].get_wcet();
    }
}
std::vector<std::pair<int, int> >  solution::get_bounds(){
    return this->bounds;
}