#ifndef CPUCLOCK_H
#define CPUCLOCK_H

#include <ctime>

class cpuClock
{
private:

    clock_t t_start;
    clock_t t_end;

public:
    cpuClock();
    ~cpuClock();
    void Start();
    void End();
    void PrintTimeElapsed();
};

#endif /* CPUCLOCK_H */
