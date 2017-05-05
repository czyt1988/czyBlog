#ifndef THREADOBJECT_H
#define THREADOBJECT_H

#include <QObject>
#include <QMutex>
class ThreadObject : public QObject
{
    Q_OBJECT
public:
    ThreadObject(QObject* parent = NULL);
    ~ThreadObject();
    void setRunCount(int count);
    void stop();
signals:
    void message(const QString& info);
    void progress(int present);
public slots:
    void runSomeBigWork1();
    void runSomeBigWork2();

private:
    int m_runCount;
    int m_runCount2;
    bool m_isStop;

    QMutex m_stopMutex;
};

#endif // THREADOBJECT_H
