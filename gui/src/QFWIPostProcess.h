#ifndef QFWIPOSTPROCESS_CPP
#define QFWIPOSTPROCESS_CPP

#include <QObject>
#include <iostream>

class QPostProcess : public QObject{
   Q_OBJECT
public:
    explicit QPostProcess (QObject* parent = 0) : QObject(parent) {}
    Q_INVOKABLE void basicPrint(){
        std::cout << "C++ PostProcess" << std::endl;
    }
};

#endif // QFWIPOSTPROCESS_CPP
