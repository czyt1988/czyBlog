#include "ThreadFromQThread.h"
#include <QDebug>
#include <QMutexLocker>
ThreadFromQThread::ThreadFromQThread(QObject* par) : QThread(par)
,m_runCount(20)
{

}

ThreadFromQThread::~ThreadFromQThread()
{
    qDebug() << "ThreadFromQThread::~ThreadFromQThread()";
}

void ThreadFromQThread::setSomething()
{
    msleep(500);
    QString str = QString("%1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((unsigned int)QThread::currentThreadId());
    emit message(str);
}

void ThreadFromQThread::getSomething()
{
    msleep(500);
    emit message(QString("%1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((unsigned int)QThread::currentThreadId()));
}

void ThreadFromQThread::setRunCount(int count)
{
    m_runCount = count;
    emit message(QString("%1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((unsigned int)QThread::currentThreadId()));
}

void ThreadFromQThread::run()
{
    int count = 0;
    m_isCanRun = true;
    QString str = QString("%1->%2,thread id:%3").arg(__FILE__).arg(__FUNCTION__).arg((unsigned int)QThread::currentThreadId());
    emit message(str);
    while(1)
    {
        sleep(1);
        ++count;
        emit progress(((float)count / m_runCount) * 100);
        emit message(QString("ThreadFromQThread::run times:%1").arg(count));
        doSomething();
        if(m_runCount == count)
        {
            break;
        }

        {
            QMutexLocker locker(&m_lock);
            if(!m_isCanRun)
            {
                return;
            }
        }
    }
}

void ThreadFromQThread::doSomething()
{
    msleep(500);
    emit message(QString("%1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((unsigned int)QThread::currentThreadId()));
}


void ThreadFromQThread::stopImmediately()
{
    QMutexLocker locker(&m_lock);
    m_isCanRun = false;
}

