#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include <fstream>
#include <vector>
#include <string>
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
    std::vector<int> values;
    for(xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()){
        values.push_back(atoi(attr->value()));
    }
    this->taskindex = values[0];
    this->wcet = values[4];
    this->bcet = values[3];
    this->prio = values[2];
    this->period = values[5];
    this->procnum = values[7];
    this->maj_fr = values[1];
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
int task::get_maj_fr(){
    return this->maj_fr;
}
void task::find_anomaltasks(){
    // запустить поиск аномальных задач, записать информацию о них в вектор
}
int main(){
    task n(1);
    std::cout << n.get_taskindex() << std::endl;

}