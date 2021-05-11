#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <charconv>
#include <algorithm>
#include <utility>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "task.h"
#include "solution.h"
using namespace rapidxml;
void create_inputfile_ue(std::string path, int tasknum, int val){
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

void create_inputfile_le(std::string path, int tasknum, int val){
    // std::cout << "there1\n";
    xml_document<char> doc;
    std::ifstream file(path);
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    // std::cout << "there2\n";
    xml_node<> *root_node = doc.first_node("system");
    // std::cout << "there3\n";
    xml_node<> *node1 = root_node->first_node("module");
    // std::cout << "there4\n";
    for(; node1; node1 = node1->next_sibling()){
        // std::cout << "there5\n";
        if(node1->first_node() != NULL){
            xml_node<> *node2 = node1->first_node("partition");
            xml_node<> *node3 = node2->first_node();
            // std::cout << "there6\n";
            for(; node3; node3 = node3->next_sibling()){
                // std::cout << "there7\n";
                xml_attribute<> *attr = node3->first_attribute();
                // std::cout << "there8\n" << attr->value() << std::endl;
                if(atoi(attr->value()) != tasknum){
                    // std::cout << "there9\n";
                    continue;
                }
                else{
                    // std::cout << "there10\n";
                    std::string s = std::to_string(val);
                    // std::cout << "there11\n";
                    const char * text = doc.allocate_string(s.c_str(), strlen(s.c_str()));
                    // std::cout << "there12\n";
                    node3->first_attribute("wcet")->value(text);
                    // std::cout << "there13\n";
                }
            }
        }
    }
    std::string data;
    rapidxml::print(std::back_inserter(data), doc);
    // std::cout << "there\n";
    std::ofstream file2;
    file2.open(path.c_str());
    file2 << data;
    file2.close();
}

void find_solution(task tmp1, solution tmp2, std::vector<task> anom_tasks, std::vector<solution> anom_sol, int & WCRT, std::string path1, std::string path2){
    // std::cout << "1\n";
    tmp2.get_lower_estimate(path2);
    int opt = tmp2.get_le();
    tmp2.get_upper_estimate(path1);
    int est = tmp2.get_ue();
    system("cp input.xml input1.xml");
    system("cp data.xml data1.xml");
    std::vector<int> wb;
    
    for(int i = 0; i < anom_tasks.size(); i++){
        // std::cout << "2\n";
        std::cout << "wcet: " << anom_tasks[i].get_wcet() << " bcet: " << anom_tasks[i].get_bcet() << "\n";
        // for(int j = anom_tasks[i].get_wcet(); j >= anom_tasks[i].get_bcet(); j--){
        //     // std::cout << "here2\n";
        //     create_inputfile_le(std::string("data1.xml"), i, j);
        //     // std::cout << "here3\n";
        //     tmp2.get_lower_estimate("data1.xml");
        //     int opt_tmp = tmp2.get_le();
        //     if(opt_tmp > opt){
        //         opt = opt_tmp;
        //     }
        // }
        for(int j = anom_tasks[i].get_wcet(); j >= anom_tasks[i].get_bcet(); j--){
            wb.push_back(j);
        }
        for(int j = 0; j < wb.size(); j++){
            // std::cout << "3\n";
            create_inputfile_ue(std::string("input1.xml"), i, j);
            tmp2.get_upper_estimate("input1.xml");
            est = tmp2.get_ue();
            std::cout << i  << " " << j << " " << opt << " " << est << "\n";
            if(est >= opt){
                opt = est;
                break;
            }
            else{
                wb.erase(std::remove(wb.begin(), wb.end(), wb[j]), wb.end());
            }
        }
    }
    WCRT = opt;
}

int main(){
    std::string path1("input.xml");
    std::string path2("data.xml");
    task tmp1(19);
    solution tmp2(19);
    tmp2.get_lower_estimate(path2);
    tmp2.get_upper_estimate(path1);
    std::cout << tmp2.get_le() << " " << tmp2.get_ue() << "\n";
    tmp1.find_anomaltasks();
    std::cout << 11 << "\n";
    std::vector<task> anom_tasks;
    std::vector<solution> anom_sol;
    std::vector<int> anom_t_ind = tmp1.get_anomaltasks();
    // std::cout << anom_t_ind[0] << "\n";
    for(int i = 0; i < anom_t_ind.size(); i++){
        anom_tasks.push_back(task(anom_t_ind[i]));
    }
    for(int i = 0; i < anom_t_ind.size(); i++){
        anom_sol.push_back(solution(anom_t_ind[i]));
    }
    int WCRT;
    std::cout << "here\n";
    find_solution(tmp1, tmp2, anom_tasks, anom_sol, WCRT, path1, path2);
    std::cout << WCRT << std::endl;
}