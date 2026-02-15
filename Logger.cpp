#include "Logger.h"
#include <QDateTime>
#include <iostream>
Logger* Logger::m_instance = nullptr;
QMutex Logger::m_mutex;

Logger::Logger(QObject* parent) : QObject(parent)
{
    m_logFile.setFileName("drone_log.txt");
    if (m_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        m_stream.setDevice(&m_logFile);
    }
}

Logger::~Logger()
{
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
}

Logger* Logger::instance()
{
    QMutexLocker locker(&m_mutex);
    if (!m_instance) {
        m_instance = new Logger();
    }
    return m_instance;
}

void Logger::log(const QString& message)
{
    QMutexLocker locker(&m_mutex);
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString formatted = QString("[%1] %2").arg(timestamp, message);

    qDebug() << formatted;

    if (m_logFile.isOpen()) {
        m_stream << formatted << "\n";
    }
}

void Logger::error(const QString& message)
{
    log("ERROR: " + message);
}
