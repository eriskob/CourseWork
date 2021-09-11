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
#include <chrono>
using namespace rapidxml;
int find_num_of_proc(std::string inputfile){
    system_config system(inputfile);
    std::vector<task> tasks;
    for(int i = 0; i < system.get_number_of_tasks(); i++){
        tasks.push_back(task(i, inputfile.c_str()));
    }
    int t = 0;
    for(int i = 0; i < tasks.size(); i++){
        if(tasks[i].get_procnum() >= t){
            t = tasks[i].get_procnum();
        }
    }
    return t;
}
void create_datafile(std::string inputfile){
    system_config system(inputfile);
    std::vector<task> tasks;
    for(int i = 0; i < system.get_number_of_tasks(); i++){
        tasks.push_back(task(i, inputfile));
    }
    xml_document<char> doc;
    std::ifstream file(inputfile);
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    xml_node<> *root_node = doc.first_node("system");
    xml_node<> *node = root_node->first_node();
    xml_document<> doc1;
    std::ofstream file_stored("data.xml");
    xml_node<> *node_wr1 = doc1.allocate_node(node_element, "system");
    doc1.append_node(node_wr1);
    int proc = find_num_of_proc(inputfile);
    std::cout << "proc: " << proc << "\n";
    for(int i = 0; i <= proc; i++){
        xml_node<> *node_wr2 = doc1.allocate_node(node_element, "module");
        node_wr1->append_node(node_wr2);
        node_wr2->append_attribute(doc.allocate_attribute("major_frame", std::to_string(system.get_maj_fr()).c_str()));
        std::string *module = new std::string("Module");
        *module += std::to_string(i);
        char *node_name = doc.allocate_string(module->c_str());
        node_wr2->append_attribute(doc.allocate_attribute("name", node_name));
        delete module;
        xml_node<> *node_wr3 = doc1.allocate_node(node_element, "partition");
        node_wr2->append_node(node_wr3);
        node_wr3->append_attribute(doc.allocate_attribute("id", "0"));
        node_wr3->append_attribute(doc.allocate_attribute("name", "m0_part0"));
        node_wr3->append_attribute(doc.allocate_attribute("scheduler", "FPPS"));
        for(int j = 0; j < tasks.size(); j++){
            if(tasks[j].get_procnum() == i){
                xml_node<> *node_wr4 = doc1.allocate_node(node_element, "task");
                node_wr3->append_node(node_wr4);
                char *node_name4 = doc.allocate_string(std::to_string(tasks[j].get_taskindex()).c_str());
                node_wr4->append_attribute(doc.allocate_attribute("id", node_name4));
                std::string name5 = std::string("task");
                name5 += std::to_string(tasks[j].get_taskindex());
                char *node_name5 = doc.allocate_string(name5.c_str());
                node_wr4->append_attribute(doc.allocate_attribute("name", node_name5));
                char *node_name6 = doc.allocate_string(std::to_string(tasks[j].get_wcet()).c_str());
                node_wr4->append_attribute(doc.allocate_attribute("wcet", node_name6));
                char *node_name7 = doc.allocate_string(std::to_string(tasks[j].get_prio()).c_str());
                node_wr4->append_attribute(doc.allocate_attribute("prio", node_name7));
                node_wr4->append_attribute(doc.allocate_attribute("offset", "0"));
                char *node_name8 = doc.allocate_string(std::to_string(tasks[j].get_period()).c_str());
                node_wr4->append_attribute(doc.allocate_attribute("period", node_name8));
                char *node_name9 = doc.allocate_string(std::to_string(tasks[j].get_deadline()).c_str());
                node_wr4->append_attribute(doc.allocate_attribute("deadline", node_name9));
            }
        }
        xml_node<> *node_wr5 = doc1.allocate_node(node_element, "window");
        node_wr2->append_node(node_wr5);
        node_wr5->append_attribute(doc.allocate_attribute("start", "0"));
        char *node_name9 = doc.allocate_string(std::to_string(tasks[0].get_deadline()).c_str());
        node_wr5->append_attribute(doc.allocate_attribute("stop", node_name9));
        node_wr5->append_attribute(doc.allocate_attribute("partition", "0"));
    }
    for(int i = 0; i < system.get_number_of_tasks(); i++){
        node = node->next_sibling();
    }
    for(; node; node = node->next_sibling()){
        xml_node<> *node_wr6 = doc1.allocate_node(node_element, "link");
        node_wr1->append_node(node_wr6);
        node_wr6->append_attribute(doc.allocate_attribute("src", node->first_attribute("src")->value()));
        node_wr6->append_attribute(doc.allocate_attribute("dst", node->first_attribute("dist")->value()));
        node_wr6->append_attribute(doc.allocate_attribute("delay", node->first_attribute("delay")->value()));
    }
    file_stored << doc1;
    file_stored.close();
    doc1.clear();
}
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

    char * text = doc.allocate_string(s.c_str());
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
    xml_document<char> doc;
    std::ifstream file(path);
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    xml_node<> *root_node = doc.first_node("system");
    xml_node<> *node1 = root_node->first_node("module");
    for(; node1; node1 = node1->next_sibling()){
        if(node1->first_node() != NULL){
            xml_node<> *node2 = node1->first_node("partition");
            xml_node<> *node3 = node2->first_node();
            for(; node3; node3 = node3->next_sibling()){
                xml_attribute<> *attr = node3->first_attribute();
                if(atoi(attr->value()) != tasknum){
                    continue;
                }
                else{
                    std::string s = std::to_string(val);
                    char * text = doc.allocate_string(s.c_str());
                    node3->first_attribute("wcet")->value(text);
                }
            }
        }
    }
    std::string data;
    rapidxml::print(std::back_inserter(data), doc);
    std::ofstream file2;
    file2.open(path.c_str());
    file2 << data;
    file2.close();
}

void rec(std::vector<task> anom_tasks, int & opt, int & est, task tmp1, solution tmp2, std::string path1){
    std::cout << "next:\n";
    std::cout << anom_tasks[0].get_bcet() << " " << anom_tasks[0].get_wcet() << "\n";
    for(int j = anom_tasks[0].get_wcet(); j >= anom_tasks[0].get_bcet(); j--){
        std::cout << "\n task number: " << anom_tasks[0].get_taskindex() << " wcet: " << j << "\n";
        create_inputfile_le(std::string("data1.xml"), anom_tasks[0].get_taskindex(), j);
        // std::cout << "here3\n";
        tmp2.get_lower_estimate("data1.xml", path1);
        int opt_tmp = tmp2.get_le();
        std::cout << "opt: " << opt_tmp << "\n";
        if(opt_tmp > opt){
            opt = opt_tmp;
        }
    }
    for(int j = anom_tasks[0].get_wcet(); j >= anom_tasks[0].get_bcet(); j--){
        std::cout << anom_tasks[0].get_taskindex() << " " << anom_tasks[0].get_wcet() << " " << anom_tasks[0].get_bcet() << " curr: " << j << " \n";
        create_inputfile_ue("input1.xml", anom_tasks[0].get_taskindex(), j);
        tmp2.get_upper_estimate("input1.xml");
        est = tmp2.get_ue();
        if(est > opt){
            opt = est;
            std::vector<task> anom_tasks_copy = anom_tasks;
            anom_tasks_copy.erase(anom_tasks_copy.begin());
            if(anom_tasks_copy.size() != 0){
                rec(anom_tasks_copy, opt, est, tmp1, tmp2, path1);
            }
            else{
                std::cout << "end\n";
                return;
            }
        }
    }
    return;
}

void find_solution(task tmp1, solution tmp2, std::vector<task> anom_tasks, std::vector<solution> anom_sol, int & WCRT, std::string path1, std::string path2){
    tmp2.get_lower_estimate(path2, path1);
    int opt = tmp2.get_le();
    tmp2.get_upper_estimate(path1);
    int est = tmp2.get_ue();
    std::cout << opt << " " << est << "\n";
    std::string path1_cp = "cp " + path1 + " input1.xml";
    std::string path2_cp = "cp " + path2 + " data1.xml";
    system(path1_cp.c_str());
    system(path2_cp.c_str());
    std::vector<int> wb;
    if(opt == est){
        WCRT = opt;
        return;
    }
    rec(anom_tasks, opt, est, tmp1, tmp2, path1);
    
    WCRT = opt;
}

int main(){
    int WCRT;
    std::string path1;
    std::cout << "enter input file name: ";
    std::cin >> path1;
    create_datafile(path1);
    std::string path2("./data.xml");
    std::cout << "enter task number: ";
    int task_number;
    std::cin >> task_number;
    task tmp1(task_number, path1.c_str());
    task tmp3(0, path1.c_str());
    std::cout << tmp3.get_wcet() << "\n";
    solution tmp2(task_number);
    tmp2.get_lower_estimate(path2, path1);
    tmp2.get_upper_estimate(path1);
    std::cout << "lower and upper: " << tmp2.get_le() << " " << tmp2.get_ue() << "\n";
    tmp1.find_anomaltasks(path1);
    std::cout << "anom tasks size: " << tmp1.get_anomaltasks().size() << std::endl;
    std::vector<task> anom_tasks;
    std::vector<solution> anom_sol;
    auto started = std::chrono::high_resolution_clock::now();
    if(tmp1.get_anomaltasks().size() != 0){
        std::vector<int> anom_t_ind = tmp1.get_anomaltasks();
        std::cout << anom_t_ind[0] << "\n";
        for(int i = 0; i < anom_t_ind.size(); i++){
            anom_tasks.push_back(task(anom_t_ind[i], path1));
        }
        for(int i = 0; i < anom_t_ind.size(); i++){
            anom_sol.push_back(solution(anom_t_ind[i]));
        }
        std::cout << "here\n";
        find_solution(tmp1, tmp2, anom_tasks, anom_sol, WCRT, path1, path2);
    }
    else{
        WCRT = tmp2.get_le();
    }
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << "anom tasks size: " << tmp1.get_anomaltasks().size() << std::endl;
    std::cout << "lower and upper: " << tmp2.get_le() << " " << tmp2.get_ue() << "\n";
    std::cout << "WCRT: " << WCRT << std::endl;
    std::cout << "time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << " ms" << std::endl;
}