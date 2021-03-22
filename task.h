#include <vector>

class task{
        int taskindex;
        int bcet;
        int wcet;
        int prio;
        int period;
        int procnum;
        int maj_fr;
        std::vector<int> anomaltasks;
    public:
        task(int num);
        int get_taskindex();
        int get_wcet();
        int get_bcet();
        int get_prio();
        int get_period();
        int get_procnum();
        int get_maj_fr();
        void find_anomaltasks();
};