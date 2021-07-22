#ifndef QFWIPOSTPROCESS_CPP
#define QFWIPOSTPROCESS_CPP

#include <QObject>
#include <QProcess>
#include <iostream>

class Launcher : public QObject{
   Q_OBJECT
protected:
    QProcess *m_process;
public:
    explicit Launcher (QObject* parent = 0) : QObject(parent), m_process(new QProcess(this)) {}

    Q_INVOKABLE int exec(QString command) const {
        std::string stdCommand = command.toStdString();
        std::cout << "Input: " << stdCommand << std::endl;

        char* char_arr = &stdCommand[0];
        return system(char_arr);
    }

    Q_INVOKABLE QString exec2(const QString &command) const {
        m_process->start(command);
//        m_process->waitForFinished(-1);
        QByteArray bytes = m_process->readAllStandardOutput();
        QString output = QString::fromLocal8Bit(bytes);
        return output;
    }
};

#endif // QFWIPOSTPROCESS_CPP
