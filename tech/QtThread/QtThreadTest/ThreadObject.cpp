#include "ThreadObject.h"
#include <QThread>
ThreadObject::ThreadObject(QObject *parent):QObject(parent)
  ,m_runCount(10)
{

}

ThreadObject::~ThreadObject()
{
    emit message(QString("Destroy %1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((int)QThread::currentThreadId()));
}

void ThreadObject::setRunCount(int count)
{
    m_runCount = count;
    emit message(QString("%1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((int)QThread::currentThreadId()));
}

void ThreadObject::runSomeBigWork()
{
    int count = 0;
    QString str = QString("%1->%2,thread id:%3").arg(__FILE__).arg(__FUNCTION__).arg((int)QThread::currentThreadId());
    emit message(str);
    while(1)
    {
        sleep(1);
        emit progress(((float)count / m_runCount) * 100);
        emit message(QString("Object::run times:%1,m_runCount:%2").arg(count).arg(m_runCount));
        ++count;
        if(m_runCount == count)
        {
            break;
        }
    }
}

