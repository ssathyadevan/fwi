#include "progressBar.h"

progressBar::~progressBar(){
    std::cerr << std::endl;
}

progressBar::progressBar(const int total, const int counter): _total(total), _counter(counter){
    _terminalWidth = 80;//findTerminalSize();
    initBar();
}

// int progressBar::findTerminalSize(){
// #ifdef __unix__
//     struct winsize size;
//     ioctl(STDERR_FILENO, TIOCGWINSZ, &size);
//     return size.ws_col;
// #else
//     std::cerr << "Progress bar disbled on windows";
//     return 80;
// #endif
// }

void progressBar::initBar() const{
    std::cerr  << "      out of " << std::setw(5) << _total;// << "[" << std::setw(5) << _counter;
}

void progressBar::setCounter(const int counter){
    _counter = counter;
}

void progressBar::setTerminalWidth(const int width){
    _terminalWidth = width;
}

void progressBar::setTotal(const int total){
    _total = total;
    initBar();
}

int progressBar::getCounter() const{
    return _counter;
}

int progressBar::getTotal() const{
    return _total;
}

progressBar &progressBar::operator++(){
    _counter++;
    print();
    return *this;
}

progressBar &progressBar::operator++(int)
{
   return ++*this;
}

// Define prefix decrement operator.
progressBar &progressBar::operator--()
{
   _counter--;
   print();
   return *this;
}

// Define postfix decrement operator.
progressBar &progressBar::operator--(int)
{
   return --*this;
}

void progressBar::print() const{
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