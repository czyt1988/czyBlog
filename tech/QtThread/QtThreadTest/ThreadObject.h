#ifndef THREADOBJECT_H
#define THREADOBJECT_H

#include <QObject>

class ThreadObject : public QObject
{
    Q_OBJECT
public:
    ThreadObject(QObject* parent = NULL);
    ~ThreadObject();
    void setRunCount(int count);
signals:
    void message(const QString& info);
    void progress(int present);
public slots:
    void runSomeBigWork();
private:
    int m_runCount;
};

#endif // THREADOBJECT_H
