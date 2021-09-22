#ifndef QFWIPOSTPROCESS_CPP
#define QFWIPOSTPROCESS_CPP

#include <QObject>
#include <QProcess>
#include <iostream>
#include <QCoreApplication>
#include "fileio.h"
#include <QDesktopServices>
#include <QUrl>

class Launcher : public QObject{
   Q_OBJECT
protected:
    QProcess *m_process;
public:
    explicit Launcher (QObject* parent = 0) : QObject(parent), m_process(new QProcess(this)) {}

    Q_INVOKABLE int exec(QString command) const {
        QCoreApplication::processEvents();
        std::string stdCommand = command.toStdString();
        std::cout << "Input: " << stdCommand << std::endl;

        char* char_arr = &stdCommand[0];
        return system(char_arr);
    }

    Q_INVOKABLE QString exec2(const QString &command) const {
        QCoreApplication::processEvents();
        m_process->start(command);
        m_process->waitForFinished(-1);
        QByteArray bytes = m_process->readAllStandardOutput();
        QString output = QString::fromLocal8Bit(bytes);
        return output;
    }

    Q_INVOKABLE void openWiki() {
        QDesktopServices::openUrl(QUrl("https://redmine.alten.nl/projects/parallelized-fwi/wiki/Parallelization#GUI-Linux-only", QUrl::TolerantMode));
    }


};

#endif // QFWIPOSTPROCESS_CPP
