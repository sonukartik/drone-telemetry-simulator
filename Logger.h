#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QDebug>

class Logger : public QObject
{
    Q_OBJECT

public:
    static Logger* instance();

    void log(const QString& message);
    void error(const QString& message);

private:
    explicit Logger(QObject* parent = nullptr);
    ~Logger();

    static Logger* m_instance;
    static QMutex m_mutex;

    QFile m_logFile;
    QTextStream m_stream;
};

#endif // LOGGER_H
