#include "progressBar.h"

ProgressBar::~ProgressBar(){
    std::cerr << std::endl;
}

ProgressBar::ProgressBar(const int total, const int counter): _total(total), _counter(counter){
    _terminalWidth = 80;//findTerminalSize();
    initBar();
}

// int ProgressBar::findTerminalSize(){
// #ifdef __unix__
//     struct winsize size;
//     ioctl(STDERR_FILENO, TIOCGWINSZ, &size);
//     return size.ws_col;
// #else
//     std::cerr << "Progress bar disbled on windows";
//     return 80;
// #endif
// }

void ProgressBar::initBar() const{
    std::cerr  << "      out of " << std::setw(5) << _total;// << "[" << std::setw(5) << _counter;
}

void ProgressBar::setCounter(const int counter){
    _counter = counter;
}

void ProgressBar::setTerminalWidth(const int width){
    _terminalWidth = width;
}

void ProgressBar::setTotal(const int total){
    _total = total;
    initBar();
}

int ProgressBar::getCounter() const{
    return _counter;
}

int ProgressBar::getTotal() const{
    return _total;
}

ProgressBar& ProgressBar::operator++(){
    _counter++;
    print();
    return *this;
}

ProgressBar& ProgressBar::operator++(int)
{
   return ++*this;
}

// Define prefix decrement operator.
ProgressBar& ProgressBar::operator--()
{
   _counter--;
   print();
   return *this;
}

// Define postfix decrement operator.
ProgressBar& ProgressBar::operator--(int)
{
   return --*this;
}

void ProgressBar::print() const{
    std::cerr << "\r";
    //int i;
    //int barWidth = _terminalWidth - (5 + 5 + 2 + 7); //Total: 5char[.............5char]
    //double portion = double(_counter) / double(_total);
    std::cerr << std::setw(5) << _counter;// << "/" << std::setw(5) << _total << "[";
    ///for (i = 0; i < int(portion * barWidth); i++) std::cerr << "\u001b[32m=";
    //std::cerr << "\u001b[32m>\u001b[0m";
    //for (++i; i < barWidth; i++) std::cerr << " ";
    //std::cerr << "]" << std::setw(5) << std::fixed << std::setprecision(1) << portion * 100 << "%   " << std::endl;
}