#pragma once

#include <iostream>
#include <iomanip>

#ifdef __unix__
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> //STDOUT_FILENO
#endif

class ProgressBar{
private:
    int _total;
    int _counter;
    int _terminalWidth;
    int findTerminalSize();
    void hideCursor();
    void showCursor();

public:
    ~ProgressBar();
    ProgressBar (const int total = 100, const int counter = 0);
    void setCounter(const int counter);
    void setTerminalWidth(const int width);
    void setTotal(const int total);
    int getCounter();
    int getTotal();
    void print();
    ProgressBar& operator++();
    ProgressBar& operator++(int);
    ProgressBar& operator--(); 
    ProgressBar& operator--(int); 
};