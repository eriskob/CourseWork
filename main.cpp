#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <charconv>
#include <utility>
#include "task.h"
#include "solution.h"

int main(){
    system_config k;
    task n(29);
    std::cout << n.get_taskindex() << " " << k.get_number_of_tasks() << std::endl;
    // n.find_anomaltasks();
    solution s(29);
    std::vector<std::pair<int, int> > sol_vec = s.get_bounds();
    for(int i = 0; i < sol_vec.size(); i++){
        std::cout << sol_vec[i].first << " " << sol_vec[i].second << std::endl;
    }
}