#ifndef QFWIPREPROCESS_CPP
#define QFWIPREPROCESS_CPP

#include <QObject>
#include <QString>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

class QPreProcess : public QObject{
   Q_OBJECT
public:
    explicit QPreProcess (QObject* parent = 0) : QObject(parent) {}
//    Q_INVOKABLE QString exec(const QString cmd) const {
//        std::array<char, 128> buffer;
//        std::string result;
//        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
//        if (!pipe) {
//            throw std::runtime_error("popen() failed!");
//        }
//        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
//            result += buffer.data();
//        }
//        return result;
//        return QString::fromStdString(result);
//
//    }
};

#endif // QFWIPREPROCESS_CPP
