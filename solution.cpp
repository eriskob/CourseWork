#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "rapidxml/rapidxml.hpp"
#include <utility>
#include "task.h"
#include "solution.h"
using namespace rapidxml;
solution::solution(int tasknum){
    this->task_num = tasknum;
    task target(tasknum);
    std::vector<task> anomal_tasks;
    // target.find_anomaltasks();
    // this->num = target.get_anomaltasks().size();
    // for(int i = 0; i < num; i++){
    //     anomal_tasks.push_back(target.get_anomaltasks()[i]);
    // }
    // for(int i = 0; i < num; i++){
    //     bounds.push_back(std::pair<int, int>());
    //     bounds[i].first = anomal_tasks[i].get_bcet();
    //     bounds[i].second = anomal_tasks[i].get_wcet();
    // }
}
std::vector<std::pair<int, int> >  solution::get_bounds(){
    return this->bounds;
}
void solution::get_lower_estimate(std::string path_file){
    this->lower_est = 0;
    task tmp(this->task_num);
    std::string path = std::string("./WCRT/IMASimulator/generator/model_builder/model_builder " + path_file + " > scenario.xml");
    system(path.c_str());
    xml_document<char> doc;
    std::ifstream file("../CourseWorkpubl-master/scenario.xml");
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    xml_node<> *root_node = doc.first_node();
    xml_node<> *node = root_node->first_node();
    for(; node; node = node->next_sibling()){
        xml_attribute<> *attr = node->first_attribute();
        if(atoi(attr->value()) != this->task_num){
            continue;
        }
        else{
            for(xml_node<> *node1 = node->first_node(); node1; node1 = node1->next_sibling()){
                for(xml_node<> *node2 = node1->first_node(); node2; node2 = node2->next_sibling()){
                    xml_attribute<> *attr = node2->first_attribute();
                    if(std::string(attr->value()) == "finished"){
                        this->lower_est += atoi(attr->next_attribute()->value());
                    }
                }
            }
        }
    }
    if(tmp.get_period() < this->lower_est){
        this->lower_est -= tmp.get_period();
    }
    // std::cout << this->lower_est << "\n";
}
int solution::get_le(){
    return this->lower_est;
}
int solution::get_ue(){
    return this->upper_est;
}
void solution::get_upper_estimate(std::string path_file){
    std::string buffer, num, path = "../Course_work-master/novel_algorithm " + path_file + " " + std::to_string(this->task_num) + " >./output.txt";
    system(path.c_str());
    std::ifstream requests("./output.txt");
    std::string name("WCRT");
    std::vector <long> vals;
    int count = 0;
    while(std::getline(requests, buffer)){
        if (!(buffer.empty())){
            for(count = 0; !isalpha(buffer[count]); count++);
            int i = 0;
            if(buffer[count] == name.at(0)){
                for(i = 0; i < name.length() && buffer[count] == name.at(i); i++, count++);
            }
            if(i == name.length()){
                for(; !isdigit(buffer[count]); count++);
                for(; isdigit(buffer[count]); count++)
                    num += buffer[count];
                vals.push_back(std::stol(num));
            }
            num.clear();
        }
    }
    this->upper_est = vals[0];
    // std::cout << this->upper_est << "\n";
}