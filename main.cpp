#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <charconv>
#include <utility>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "task.h"
#include "solution.h"
using namespace rapidxml;
void create_inputfile(std::string path, int tasknum, int val){
    xml_document<char> doc;
    std::ifstream file(path);
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    xml_node<> *root_node = doc.first_node("system");
    xml_node<> *node = root_node->first_node();
    for(int i = 0; i < tasknum; i++){
        node = node->next_sibling();
    }
    std::string s = std::to_string(val);
    const char * text = doc.allocate_string(s.c_str(), strlen(s.c_str()));
    node->first_attribute("wcet")->value(text);
    node->first_attribute("bcet")->value(text);
    std::string data;
    rapidxml::print(std::back_inserter(data), doc);

    std::ofstream file2;
    file2.open(path.c_str());
    file2 << data;
    file2.close();
}

void find_solution(task tmp1, solution tmp2, std::vector<task> anom_tasks, std::vector<solution> anom_sol, int & WCRT, std::string path){
    // std::cout << "1\n";
    tmp2.get_lower_estimate();
    int opt = tmp2.get_le();
    tmp2.get_upper_estimate(path);
    int est = tmp2.get_ue();
    system("cp input.xml input1.xml");
    for(int i = 0; i < anom_tasks.size(); i++){
        std::cout << "2\n";
        std::cout << anom_tasks[i].get_bcet() << " " << anom_tasks[i].get_wcet() << "\n";
        for(int j = anom_tasks[i].get_wcet(); j >= anom_tasks[i].get_bcet(); j--){
            // std::cout << "3\n";
            create_inputfile(std::string("input1.xml"), i, j);
            tmp2.get_upper_estimate("input1.xml");
            est = tmp2.get_ue();
            std::cout << j << " " << opt << " " << est << "\n";
            if(est >= opt){
                opt = est;
                break;
            }
        }
    }
    WCRT = est;
}

int main(){
    std::string path("input.xml");
    task tmp1(29);
    solution tmp2(29);
    tmp2.get_lower_estimate();
    tmp2.get_upper_estimate(path);
    std::cout << tmp2.get_le() << " " << tmp2.get_ue() << "\n";
    tmp1.find_anomaltasks();
    std::vector<task> anom_tasks;
    std::vector<solution> anom_sol;
    std::vector<int> anom_t_ind = tmp1.get_anomaltasks();
    std::cout << anom_t_ind[0] << "\n";
    for(int i = 0; i < anom_t_ind.size(); i++){
        anom_tasks.push_back(task(anom_t_ind[i]));
    }
    for(int i = 0; i < anom_t_ind.size(); i++){
        anom_sol.push_back(solution(anom_t_ind[i]));
    }
    int WCRT;
    find_solution(tmp1, tmp2, anom_tasks, anom_sol, WCRT, path);
    std::cout << WCRT << std::endl;
}