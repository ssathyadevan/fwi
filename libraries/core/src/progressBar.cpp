#include "progressBar.h"

ProgressBar::~ProgressBar(){
    showCursor();
}

ProgressBar::ProgressBar(const int total, const int counter): _total(total), _counter(counter){
    _terminalWidth = findTerminalSize();
    hideCursor();
}

void ProgressBar::hideCursor(){
#ifdef __unix__
    std::cerr<< "\e[?25l" << std::endl; //hide cursor and leave some space
#endif
}

void ProgressBar::showCursor(){
#ifdef __unix__
    std::cerr<< "\e[?25h"; //show cursor
#endif
}

int ProgressBar::findTerminalSize(){
#ifdef __unix__
    struct winsize size;
    ioctl(STDERR_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
#else
    std::err << "Progress bar disbled on windows";
    return 80;
#endif
}

void ProgressBar::setCounter(const int counter){
    _counter = counter;
}

void ProgressBar::setTerminalWidth(const int width){
    _terminalWidth = width;
}

void ProgressBar::setTotal(const int total){
    _total = total;
}

const int ProgressBar::getCounter(){
    return _counter;
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

const void ProgressBar::print(){
#ifdef __unix__
    std::cerr << "\033[1A";
    std::cerr << "\033[1A";
    int i;
    int barWidth = _terminalWidth - (5 + 5 + 4 + 6); //counter + total + percentage + other characters
    double portion = double(_counter) / double(_total);
    std::cerr << std::setw(5) << _counter << "/" << std::setw(5) << _total << "[";
    for (i = 0; i < int(portion * barWidth); i++) std::cerr << "\u001b[32m=";
    std::cerr << "\u001b[32m>\u001b[0m";
    for (++i; i < barWidth; i++) std::cerr << " ";
    std::cerr << "]" << std::setw(5) << std::fixed << std::setprecision(1) << portion * 100 << "%   " << std::endl;
#endif
}