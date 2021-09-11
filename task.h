#ifndef TASK_H
#define TASK_H
#include <vector>

class task{
        int taskindex;
        int bcet;
        int wcet;
        int prio;
        int period;
        int procnum;
        int deadline;
        std::vector<int> anomaltasks;
    public:
        task(int num, std::string inputfile);
        int get_taskindex();
        int get_deadline();
        int get_wcet();
        int get_bcet();
        int get_prio();
        int get_period();
        int get_procnum();
        void find_anomaltasks(std::string inpufile);
        std::vector<int> get_anomaltasks();
};
class system_config{
        int tasks;
        int maj_fr;
    public:
        system_config(std::string path);
        int get_number_of_tasks();
        int get_maj_fr();
};
#endif