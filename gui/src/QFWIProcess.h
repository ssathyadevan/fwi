#ifndef QFWIPROCESS_CPP
#define QFWIPROCESS_CPP

#include <QObject>
#include <iostream>

class QProcess : public QObject{
   Q_OBJECT
public:
    explicit QProcess (QObject* parent = 0) : QObject(parent) {}
    Q_INVOKABLE void basicPrint(){
        std::cout << "C++ call print" << std::endl;
    }
};

#endif // QFWIPROCESS_CPP
