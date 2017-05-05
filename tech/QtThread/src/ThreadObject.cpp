#include "ThreadObject.h"
#include <QThread>
#include <QDebug>
#include <QMutexLocker>
#include <QElapsedTimer>
#include <limits>
ThreadObject::ThreadObject(QObject *parent):QObject(parent)
  ,m_runCount(10)
  ,m_runCount2(std::numeric_limits<int>::max())
  ,m_isStop(true)
{

}

ThreadObject::~ThreadObject()
{
    qDebug() << "ThreadObject destroy";
    emit message(QString("Destroy %1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((int)QThread::currentThreadId()));
}

void ThreadObject::setRunCount(int count)
{
    m_runCount = count;
    emit message(QString("%1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((int)QThread::currentThreadId()));
}

void ThreadObject::runSomeBigWork1()
{
    {
        QMutexLocker locker(&m_stopMutex);
        m_isStop = false;
    }
    int count = 0;
    QString str = QString("%1->%2,thread id:%3").arg(__FILE__).arg(__FUNCTION__).arg((int)QThread::currentThreadId());
    emit message(str);
    int process = 0;
    while(1)
    {
        {
            QMutexLocker locker(&m_stopMutex);
            if(m_isStop)
                return;
        }
        if(m_runCount == count)
        {
            break;
        }
        sleep(1);
        int pro = ((float)count / m_runCount) * 100;
        if(pro != process)
        {
            process = pro;
            emit progress(((float)count / m_runCount) * 100);
            emit message(QString("Object::run times:%1,m_runCount:%2").arg(count).arg(m_runCount2));
        }
        ++count;
    }
}

void ThreadObject::runSomeBigWork2()
{
    {
        QMutexLocker locker(&m_stopMutex);
        m_isStop = false;
    }
    int count = 0;
    QString str = QString("%1->%2,thread id:%3").arg(__FILE__).arg(__FUNCTION__).arg((int)QThread::currentThreadId());
    emit message(str);
    int process = 0;
    QElapsedTimer timer;
    timer.start();
    while(1)
    {
        {
            QMutexLocker locker(&m_stopMutex);
            if(m_isStop)
                return;
        }
        if(m_runCount2 == count)
        {
            break;
        }
        int pro = ((float)count / m_runCount2) * 100;
        if(pro != process)
        {
            process = pro;
            emit progress(pro);
            emit message(QString("%1,%2,%3,%4")
                         .arg(count)
                         .arg(m_runCount2)
                         .arg(pro)
                         .arg(timer.elapsed()));
            timer.restart();
        }
        ++count;
    }
}

void ThreadObject::stop()
{
    QMutexLocker locker(&m_stopMutex);
    emit message(QString("%1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((int)QThread::currentThreadId()));
    m_isStop = true;
}

