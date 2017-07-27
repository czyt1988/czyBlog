#include "Widget.h"
#include "ui_Widget.h"
#include "ThreadFromQThread.h"
#include "ThreadObject.h"
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
  ,m_objThread(NULL)
  ,m_obj(NULL)
  ,m_currentRunLoaclThread(NULL)
{
    ui->setupUi(this);
    //控件初始化
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
    ui->progressBar_heart->setRange(0,100);
    ui->progressBar_heart->setValue(0);
    //按钮的信号槽关联
    connect(ui->pushButton_qthread1,&QPushButton::clicked
            ,this,&Widget::onButtonQThreadClicked);
    connect(ui->pushButton_qthread1_setSomething,&QPushButton::clicked
            ,this,&Widget::onButtonQthread1SetSomethingClicked);
    connect(ui->pushButton_qthread1_getSomething,&QPushButton::clicked
            ,this,&Widget::onButtonQthread1GetSomethingClicked);
    connect(ui->pushButton_qthreadQuit,&QPushButton::clicked
            ,this,&Widget::onButtonQthreadQuitClicked);
    connect(ui->pushButton_qthreadTerminate,&QPushButton::clicked
            ,this,&Widget::onButtonQthreadTerminateClicked);
    connect(ui->pushButton_qthreadExit,&QPushButton::clicked
            ,this,&Widget::onButtonQThreadExitClicked);
    connect(ui->pushButton_doSomthing,&QPushButton::clicked
            ,this,&Widget::onButtonQThreadDoSomthingClicked);
    connect(ui->pushButton_qthreadRunLocal,&QPushButton::clicked
            ,this,&Widget::onButtonQThreadRunLoaclClicked);
    //
    connect(ui->pushButton_qobjectStart,&QPushButton::clicked
            ,this,&Widget::onButtonObjectMove2ThreadClicked);
    connect(ui->pushButton_qobjectStart_2,&QPushButton::clicked
            ,this,&Widget::onButtonObjectMove2Thread2Clicked);
    connect(ui->pushButton_qobjectStop,&QPushButton::clicked
            ,this,&Widget::onButtonObjectThreadStopClicked);
    //
    connect(&m_heart,&QTimer::timeout,this,&Widget::heartTimeOut);
    m_heart.setInterval(100);
    //全局线程的创建
    m_thread = new ThreadFromQThread(this);
    connect(m_thread,&ThreadFromQThread::message
            ,this,&Widget::receiveMessage);
    connect(m_thread,&ThreadFromQThread::progress
            ,this,&Widget::progress);
    connect(m_thread,&QThread::finished
            ,this,&Widget::onQThreadFinished);

    m_heart.start();
}



Widget::~Widget()
{
    qDebug() << "start destroy widget";
    m_thread->stopImmediately();
    //局部线程的终结
    if(m_currentRunLoaclThread)
    {
        m_currentRunLoaclThread->stopImmediately();
    }
    m_thread->wait();
    delete ui;

    if(m_objThread)
    {
        m_objThread->quit();
    }
    m_objThread->wait();
    qDebug() << "end destroy widget";
}

void Widget::onButtonQThreadClicked()
{
    ui->progressBar->setValue(0);
    if(m_thread->isRunning())
    {
        return;
    }
    m_thread->start();
}

void Widget::progress(int val)
{
    ui->progressBar->setValue(val);
}

void Widget::receiveMessage(const QString &str)
{
    ui->textBrowser->append(str);
}

void Widget::heartTimeOut()
{
    static int s_heartCount = 0;
    ++s_heartCount;
    if(s_heartCount > 100)
    {
        s_heartCount = 0;
    }
    ui->progressBar_heart->setValue(s_heartCount);
}

void Widget::onButtonQthread1SetSomethingClicked()
{
    m_thread->setSomething();
}

void Widget::onButtonQthread1GetSomethingClicked()
{
    m_thread->getSomething();
}

void Widget::onButtonQthreadQuitClicked()
{
    ui->textBrowser->append("m_thread->quit() but not work");
    m_thread->quit();
}

void Widget::onButtonQthreadTerminateClicked()
{
    m_thread->terminate();
}

void Widget::onButtonQThreadDoSomthingClicked()
{
    m_thread->doSomething();
}

void Widget::onButtonQThreadExitClicked()
{
    m_thread->exit();
}

void Widget::onButtonQThreadRunLoaclClicked()
{
    //局部线程的创建的创建
    if(m_currentRunLoaclThread)
    {
        m_currentRunLoaclThread->stopImmediately();
    }
    ThreadFromQThread* thread = new ThreadFromQThread(NULL);
    connect(thread,&ThreadFromQThread::message
            ,this,&Widget::receiveMessage);
    connect(thread,&ThreadFromQThread::progress
            ,this,&Widget::progress);
    connect(thread,&QThread::finished
            ,this,&Widget::onQThreadFinished);
    connect(thread,&QThread::finished
            ,thread,&QObject::deleteLater);//线程结束后调用deleteLater来销毁分配的内存
    connect(thread,&QObject::destroyed,this,&Widget::onLocalThreadDestroy);
    thread->start();
    m_currentRunLoaclThread = thread;
}

void Widget::onQThreadFinished()
{
    ui->textBrowser->append("ThreadFromQThread finish");
}


void Widget::onLocalThreadDestroy(QObject *obj)
{
    if(qobject_cast<QObject*>(m_currentRunLoaclThread) == obj)
    {
        m_currentRunLoaclThread = NULL;
    }
}

void Widget::startObjThread()
{
    if(m_objThread)
    {
        return;
    }
    m_objThread= new QThread();
    m_obj = new ThreadObject();
    m_obj->moveToThread(m_objThread);
    connect(m_objThread,&QThread::finished,m_objThread,&QObject::deleteLater);
    connect(m_objThread,&QThread::finished,m_obj,&QObject::deleteLater);
    connect(this,&Widget::startObjThreadWork1,m_obj,&ThreadObject::runSomeBigWork1);
    connect(this,&Widget::startObjThreadWork2,m_obj,&ThreadObject::runSomeBigWork2);
    connect(m_obj,&ThreadObject::progress,this,&Widget::progress);
    connect(m_obj,&ThreadObject::message,this,&Widget::receiveMessage);

    m_objThread->start();
}

void Widget::onButtonObjectMove2ThreadClicked()
{
    if(!m_objThread)
    {
        startObjThread();
    }

    emit startObjThreadWork1();//主线程通过信号换起子线程的槽函数

    ui->textBrowser->append("start Obj Thread work 1");
}

void Widget::onButtonObjectMove2Thread2Clicked()
{
    if(!m_objThread)
    {
        startObjThread();
    }
    emit startObjThreadWork2();//主线程通过信号换起子线程的槽函数

    ui->textBrowser->append("start Obj Thread work 2");
}


void Widget::onButtonObjectThreadStopClicked()
{
    if(m_objThread)
    {
        if(m_obj)
        {
            m_obj->stop();
        }
    }
}




