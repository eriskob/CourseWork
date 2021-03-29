#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <charconv>
#include "task.h"
using namespace rapidxml;
task::task(int num){
    xml_document<char> doc;
    std::ifstream file("input.xml");
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    xml_node<> *root_node = doc.first_node("system");
    xml_node<> *node = root_node->first_node();
    for(int i = 0; i < num; i++){
        node = node->next_sibling();
    }
    this->taskindex = atoi(node->first_attribute("index")->value());
    this->wcet = atoi(node->first_attribute("wcet")->value());
    this->bcet = atoi(node->first_attribute("bcet")->value());
    this->prio = atoi(node->first_attribute("prio")->value());
    this->period = atoi(node->first_attribute("period")->value());
    this->procnum = atoi(node->first_attribute("proc")->value());
}
int task::get_taskindex(){
    return this->taskindex;
}
int task::get_wcet(){
    return this->wcet;
}
int task::get_bcet(){
    return this->bcet;
}
int task::get_prio(){
    return this->prio;
}
int task::get_period(){
    return this->period;
}
int task::get_procnum(){
    return this->procnum;
}
void task::find_anomaltasks(){
    std::string path = std::string("python3 ../CourseworkPubl-master/mainBS.py ") + std::string(std::to_string(this->taskindex)) + std::string(" >output.txt");
    system(path.c_str());
    std::string buffer, num, set, lil_num;
    std::ifstream requests("./output.txt");
    std::set<int> Nums;
    std::vector<int> graph;
    std::vector<std::vector<int> > meta_graph;
    std::vector<std::set<int> > set_meta_graph;
    int count = 0;
    while(std::getline(requests, buffer)){
        if(!(buffer.empty())){
            for(count = 0; buffer[count] != ' '; count++){
                num += buffer[count];
            }
            count++;
            if(buffer[count] == 's'){
                for( ; buffer[count] != ')'; count++){
                    set += buffer[count];
                }
                set += buffer[count];
                meta_graph.push_back(std::vector<int>());
            }
            else{
                if(buffer[count] == '{'){
                    for( ; buffer[count] != '}'; count++){
                        set += buffer[count];
                        if((buffer[count] <= '9') && (buffer[count] >= '0')){
                            lil_num += buffer[count];
                        }
                        else if((buffer[count] == ',') || (buffer[count] == '}')){
                            Nums.insert(std::stoi(lil_num));
                            graph.push_back(std::stoi(lil_num));
                            lil_num = "";
                        }
                    }
                    graph.push_back(std::stoi(lil_num));
                    set += buffer[count];
                    lil_num = "";
                }
                meta_graph.push_back(graph);
                set_meta_graph.push_back(Nums);
            }
            num = "";
            set = "";
            Nums.clear();
            graph.clear();
        }

    }
    auto i = meta_graph.begin();
    for( ; i != meta_graph.begin() + this->taskindex; i++);
        for(auto j = (*i).begin(); j != (*i).end(); j++){
            this->anomaltasks.push_back((*j));
        }
    for(int i = 0; i < anomaltasks.size(); i++){
        std::cout << anomaltasks[i] << " ";
    }

}
std::vector<int> task::get_anomaltasks(){
    return this->anomaltasks;
}
system_config::system_config(){
    xml_document<char> doc;
    std::ifstream file("input.xml");
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    xml_node<> *root_node = doc.first_node("system");
    xml_node<> *node = root_node->first_node();
    this->tasks = 0;
    for(int i = 0; node; i++){
        node = node->next_sibling();
        this->tasks++;
    }
    xml_node<> *node2 = root_node->first_node();
    this->maj_fr = atoi(node2->first_attribute("maj_fr")->value());
}
int system_config::get_number_of_tasks(){
    return this->tasks;
}
int system_config::get_maj_fr(){
    return this->maj_fr;
}