[TOC]

# 1.摘要
Qt有两种多线程的方法，其中一种是继承QThread的run函数，另外一种是把一个继承于QObject的类转移到一个Thread里。
Qt4.8之前都是使用继承QThread的run这种方法，但是Qt4.8之后，Qt官方建议使用第二种方法。两种方法区别不大，用起来都比较方便，但继承QObject的方法更加灵活。这里要记录的是如何正确的创建一个线程，特别是如何正确的退出一个线程。

**本文先介绍QThread的普通用法，这个用法可能网上很多文章都介绍过，如果已经了解大可跳过此节，本文重点介绍线程退出的几种方法，根据需求正确的创建和退出线程等问题。**

# 2.Qt多线程方法1 继承`QThread`

在使用继承`QThread`的`run`方法之前需要了解一条规则：

> **`QThread`只有`run`函数是在新线程里的，其他所有函数都在`QThread`生成的线程里**

**`QThread`只有`run`函数是在新线程里的**
**`QThread`只有`run`函数是在新线程里的**
**`QThread`只有`run`函数是在新线程里的**

重要的事情说3遍！！！

，如果`QThread`是在ui所在的线程里生成，那么`QThread`的其他非`run`函数都是和ui线程一样的，所以，`QThread`的继承类的其他函数尽量别要有太耗时的操作，要确保所有耗时的操作都在`run`函数里。
在UI线程下调用`QThread`的非`run`函数（其实也不应该直接调用`run`函数，而应该使用start函数），和执行普通函数无区别，这时，如果这个函数要对`QThread`的某个变量进行变更，而这个变量在`run`函数里也会被用到，这时就需要注意加锁的问题，因为可能这个变量前几毫秒刚刚在run中调用，再调用时已经被另外的线程修改了。

## 2.1写一个继承于`QThread`的线程

本文的重点不是教会你继承`run`写一个多线程，任何有编程基础的5分钟就能学会使用`QThread`的方法，本文真正要讲的是后面那几节，如如何安全的退出一个线程，如何开启一个临时线程，运行结束马上关闭等问题。如果你对`QThread`有初步了解，那么可以略过这节，但你最好看看这节后面提出的几个问题。

任何继承于`QThread`的线程都是通过继承`QThread`的`run`函数来实现多线程的，因此，必须重写`QThread`的`run`函数，把复杂逻辑写在`QThread`的`run`函数中。

看看一个普通继承`QThread`的例子：
头文件：
```cpp
#ifndef THREADFROMQTHREAD_H
#define THREADFROMQTHREAD_H
#include <QThread>

class ThreadFromQThread : public QThread
{
    Q_OBJECT
signals:
    void message(const QString& info);
    void progress(int present);
public:
    ThreadFromQThread(QObject* par);
    ~ThreadFromQThread();
    void setSomething();
    void getSomething();
    void setRunCount(int count);
    void run();
    void doSomething();
private:
    int m_runCount;
};

#endif // THREADFROMQTHREAD_H
```
cpp文件：
```cpp
#include "ThreadFromQThread.h"
#include <QDebug>
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
    QString str = QString("%1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((int)QThread::currentThreadId());
    emit message(str);
}

void ThreadFromQThread::getSomething()
{
    msleep(500);
    emit message(QString("%1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((int)QThread::currentThreadId()));
}

void ThreadFromQThread::setRunCount(int count)
{
    m_runCount = count;
    emit message(QString("%1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((int)QThread::currentThreadId()));
}

void ThreadFromQThread::run()
{
    int count = 0;
    QString str = QString("%1->%2,thread id:%3").arg(__FILE__).arg(__FUNCTION__).arg((int)QThread::currentThreadId());
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
    }
}

void ThreadFromQThread::doSomething()
{
    msleep(500);
    emit message(QString("%1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((int)QThread::currentThreadId()));    
}

```
这个简单的例子有一个Qt类常见的内容，包含了普通方法，信号槽，和一个`run`函数。这里函数`setSomething();`进行了500ms的延迟，`getSomething`同理。这是为了验证在`QThread::run()`之外调用QThread成员函数不会运行在新线程里。

上面代码用到了`QThread::currentThreadId()`这是一个静态函数，用于返回当前线程句柄，这个值除了区分以外没有别的用处。

为了验证这个线程，编写一个简单的界面，这个界面主要用于验证如下几个问题：、
- 在UI线程调用`setSomething();`函数和`getSomething();`函数会不会卡顿？
- 在UI线程调用`QThread::quit()`或`QThread::exit()`函数会不会停止线程？
- 在UI线程调用`QThread::terminate`函数会不会停止线程？
- **如何正确的退出线程？**

# 2.2 QThread的几个函数quit、exit、terminate函数

为了验证上面这些，编写一个简单的界面如下图所示：

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QtThread/pic/01.jpg)
 
```cpp
#include "Widget.h"
#include "ui_Widget.h"
#include "ThreadFromQThread.h"
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
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
    connect(ui->pushButton_objQuit,&QPushButton::clicked
            ,this,&Widget::onButtonObjectQuitClicked);
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
    m_thread->stopImmediately();//由于此线程的父对象是Widget，因此退出时需要进行判断
    m_thread->wait();
    delete ui;
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

void Widget::onQThreadFinished()
{
    ui->textBrowser->append("ThreadFromQThread finish");
}
```

界面为上面提到的几个问题提供了按钮， 界面有一个心跳进度条，它是主程序的定时器控制，每100ms触发用于证明主程序的ui线程没有卡死。第二个进度条由线程控制。

点击"QThread run"按钮，触发`onButtonQThreadClicked`槽，子线程会运行，子线程运行起来后，会打印
>../QtThreadTest/ThreadFromQThread.cpp->run,thread id:2900388672

可以确定线程运行的id是2900388672
子线程是个循环，每次循环都会有打印信息：

> ThreadFromQThread::run times:1
doSomething->../QtThreadTest/ThreadFromQThread.cpp,thread id:2900388672
ThreadFromQThread::run times:2
doSomething->../QtThreadTest/ThreadFromQThread.cpp,thread id:2900388672

`doSomething`是在`run`函数里调用，其线程id是2900388672，可见这时`doSomething`函数是运行在子线程里的。

这时，我在界面点击`getSomething`,`setSomething`,`doSomething`会打印：
>getSomething->../QtThreadTest/ThreadFromQThread.cpp,thread id:3021526784
setSomething->../QtThreadTest/ThreadFromQThread.cpp,thread id:3021526784
doSomething->../QtThreadTest/ThreadFromQThread.cpp,thread id:3021526784

说明在非run函数里调用QThread的成员函数，并不是在线程里运行(3021526784是widget所在线程)

这时我点击`quit`，thread并没进行任何处理，QThread在不调用`exec()`情况下是`exit`函数和`quit`函数是没有作用的。

> m_thread->quit() but not work

点击terminate按钮，线程马上终止，打印：

> ThreadFromQThread finish

动态图如下图所示：

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QtThread/pic/02.gif)

因此可以看出`quit`和`exit`函数都不会中途终端线程，要马上终止一个线程可以使用`terminate`函数，但这个函数存在非常不安定因素，不推荐使用。那么如何安全的终止一个线程呢？

# 2.3 正确的终止一个线程
最简单的方法是添加一个bool变量，通过主线程修改这个bool变量来进行终止，但这样有可能引起访问冲突，需要加锁
我们需要在原来的头文件加上如下语句：
```cpp
#include <QMutex>

class ThreadFromQThread : public QThread
{
...........
public slots:
    void stopImmediately();
private:
    QMutex m_lock;
    bool m_isCanRun;
.........
};
```
run函数需要进行修改：
```cpp
void ThreadFromQThread::stopImmediately()
{
    QMutexLocker locker(&m_lock);
    m_isCanRun = false;
}

void ThreadFromQThread::run()
{
    int count = 0;
    m_isCanRun = true;//标记可以运行
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
            if(!m_isCanRun)//在每次循环判断是否可以运行，如果不行就退出循环
            {
                return;
            }
        }
    }
}
```
`QMutexLocker `可以安全的使用QMutex，以免忘记解锁(有点类似std::unique_ptr)，这样每次循环都会看看是否要马上终止。
在线程需要马上退出时，可以在外部调用`stopImmediately()`函数终止线程，之前的例子可以知道，由于在主线程调用QThread非`run()`函数的函数都是在主线程运行，因此，在主线程调用类似`m_thread->stopImmediately()`会几乎马上把线程的成员变量`m_isCanRun `设置为false(面对多线程问题要用面向过程的思维思考)，因此在子线程的`run`函数的循环中遇到`m_isCanRun`的判断后就会退出`run`函数,继承QThread的函数在运行完`run`函数后就视为线程完成，会发射`finish`信号。

## 2.4 如何正确启动一个线程
线程的启动有几种方法，这几种方法设计到它的父对象归属问题，和如何删除他的问题。首先要搞清楚这个线程是否和UI的生命周期一致，直到UI结束线程才结束，还是这个线程只是临时生成，等计算完成就销毁。

第一种情况的线程在创建时会把生成线程的窗体作为它的父对象，这样窗体结束时会自动析构线程的对象。但这时候要注意一个问题，就是**窗体结束时线程还未结束如何处理**，如果没有处理这种问题，你会发现关闭窗口时会导致程序崩溃。往往这种线程是一个监控线程，如监控某个端口的线程。为了好区分，暂时叫这种叫全局线程，它在UI的生命周期中都存在。

第二种情况是一种临时线程，这种线程一般是突然要处理一个大计算，为了不让UI假死需要触发的线程，这时需要注意一个问题，就是**在线程还没计算完成，用户突然终止或变更时如何处理**，这种线程往往更多见且更容易出错，如打开一个大文件，显示一个大图片，用户可能看一个大图片还没等图片处理完成又切换到下一个图片，这时绘图线程要如何处理才能顺利解决？为了好区分，暂时叫这种叫局部线程，它在UI的生命周期中仅仅是某时刻才会触发，然后销毁。

这就涉及到如何终止正在执行的线程这个问题！


### 2.4.1正确的启动一个全局线程(和UI一直存在的线程)

我发现大部分网上的教程都是教你创建一个全局的线程，但往往这种线程用的不多，也比较好管理，需要注意的是程序退出时对线程的处理问题。
在ui的头文件中声明一个线程的指针

widget.h:
```cpp
ThreadFromQThread* m_thread;
```

wodget.cpp:
```cpp
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void onButtonQThreadClicked();
    void onButtonQthread1SetSomethingClicked();
    void onButtonQthread1GetSomethingClicked();
    void onButtonQthreadQuitClicked();
    void onButtonQthreadTerminateClicked();
    void onButtonQThreadDoSomthingClicked();
    void onQThreadFinished();
......
    void progress(int val);
    void receiveMessage(const QString& str);
    void heartTimeOut();
private:
    Ui::Widget *ui;
    ThreadFromQThread* m_thread;
    QTimer m_heart;
......
};
```

先看窗体生成的构造函数
```cpp
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
  ,m_objThread(NULL)
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
    connect(ui->pushButton_doSomthing,&QPushButton::clicked
            ,this,&Widget::onButtonQThreadDoSomthingClicked);
    //心跳的关联
    connect(&m_heart,&QTimer::timeout,this,&Widget::heartTimeOut);
    m_heart.setInterval(100);
    //全局线程的创建
    //全局线程创建时可以把窗体指针作为父对象
    m_thread = new ThreadFromQThread(this);
    //关联线程的信号和槽
    connect(m_thread,&ThreadFromQThread::message
            ,this,&Widget::receiveMessage);//
    connect(m_thread,&ThreadFromQThread::progress
            ,this,&Widget::progress);
    connect(m_thread,&QThread::finished
            ,this,&Widget::onQThreadFinished);
    //UI心跳开始
    m_heart.start();
}
```
由于是全局存在的线程，因此在窗体创建时就创建线程，可以把线程的父对象设置为窗体，这时需要注意，别手动delete线程指针。用于你的QThread是在Qt的事件循环里面，手动delete会发生不可预料的意外。理论上所有QObject都不应该手动delete，如果没有多线程，手动delete可能不会发生问题，但是多线程情况下delete非常容易出问题，那是因为**有可能你要删除的这个对象在Qt的事件循环里还排队，但你却已经在外面删除了它，这样程序会发生崩溃**。

**如果你确实要删除，请参阅`void QObject::deleteLater () [slot]`这个槽，这个槽非常有用，尤其是对局部线程来说。后面会经常用到它用于安全的结束线程。**

在需要启动线程的地方调用`start`函数即可启动线程。
```cpp
void Widget::onButtonQThreadClicked()
{
    ui->progressBar->setValue(0);
    if(m_thread->isRunning())
    {
        return;
    }
    m_thread->start();
}
```
**如果线程已经运行，你重复调用`start`其实是不会进行任何处理。**

一个全局线程就那么简单，要用的时候`start`一下就行。真正要注意的是如何在ui结束时把线程安全退出。

在widget的析构函数应该这样写：
```cpp
Widget::~Widget()
{
    qDebug() << "start destroy widget";
    m_thread->stopImmediately();
    m_thread->wait();
    delete ui;
    qDebug() << "end destroy widget";
}
```
这里要注意的是`m_thread->wait();`这一句，这一句是主线程等待子线程结束才能继续往下执行，这样能确保过程是单一往下进行的，也就是不会说子线程还没结束完，主线程就destrioy掉了(`m_thread`的父类是主线程窗口，主线程窗口如果没等子线程结束就destroy的话，会顺手把`m_thread`也`delete`这时就会奔溃了)，因此`wait`的作用就是挂起，一直等到子线程结束。

还有一种方法是让QThread自己删除自己，就是在new线程时，不指定父对象，通过绑定**`void QObject::deleteLater () [slot]`**槽让它自动释放。这样在widget析构时可以免去`m_thread->wait();`这句。

### 2.4.2 如何启动一个局部线程(用完即释放的线程)

启动一个局部线程(就是运行完自动删除的线程)方法和启动全局线程差不多，但要关联多一个槽函数，就是之前提到的**`void QObject::deleteLater () [slot]`**，这个槽函数是能安全释放线程资源的关键(直接delete thread指针不安全)。

简单的例子如下：
```cpp
void Widget::onButtonQThreadRunLoaclClicked()
{
    //局部线程的创建的创建
    ThreadFromQThread* thread = new ThreadFromQThread(NULL);//这里父对象指定为NULL
    connect(thread,&ThreadFromQThread::message
            ,this,&Widget::receiveMessage);
    connect(thread,&ThreadFromQThread::progress
            ,this,&Widget::progress);
    connect(thread,&QThread::finished
            ,this,&Widget::onQThreadFinished);
    connect(thread,&QThread::finished
            ,thread,&QObject::deleteLater);//线程结束后调用deleteLater来销毁分配的内存
    thread->start();
}
```
这个例子还是启动之前的线程，但不同的是：
- `new ThreadFromQThread(NULL);`并没有给他指定父对象
- `connect(thread,&QThread::finished ,thread,&QObject::deleteLater);`线程结束后调用deleteLater来销毁分配的内存。
再线程运行完成，发射finished信号后会调用`deleteLater`函数，在确认消息循环中没有这个线程的对象后会销毁。

但是要注意避免重复点按钮重复调用线程的情况，对于一些需求，线程开启后再点击按钮不会再重新生成线程，一直等到当前线程执行完才能再次点击按钮，这种情况很好处理，加个标记就可以实现，也一般比较少用。

另外更多见的需求是，再次点击按钮，需要终结上次未执行完的线程，重新执行一个新线程。这种情况非常多见，例如一个普通的图片浏览器，都会有下一张图和上一张图这种按钮，浏览器加载图片一般都在线程里执行(否则点击超大图片时图片浏览器会类似卡死的状态),用户点击下一张图片时需要终止正在加载的当前图片，加载下一张图片。你不能要求客户要当前图片加载完才能加载下一张图片，这就几乎沦为单线程了。这时候，就需要终止当前线程，开辟新线程加载下一个图片。

这时，上面的函数将会是大概这个样子的

UI的头文件需要一个成员变量记录正在运行的线程
```cpp
private slots：
   void onLocalThreadDestroy(QObject* obj);
private:
   QThread* m_currentRunLoaclThread;
```
运行生成临时线程的函数将变为
```cpp
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

void Widget::onLocalThreadDestroy(QObject *obj)
{
    if(qobject_cast<QObject*>(m_currentRunLoaclThread) == obj)
    {
        m_currentRunLoaclThread = NULL;
    }
}
```
这里用一个临时变量记录当前正在运行的局部线程，由于线程结束时会销毁自己，因此要通知主线程把这个保存线程指针的临时变量设置为NULL
因此用到了`QObject::destroyed`信号，在线程对象析构时通知UI把`m_currentRunLoaclThread `设置为nullptr;


## 2.5 继承`QThread`的一些总结

-  **在`QThread`执行`start`函数之后，`run`函数还未运行完毕，再次`start`会出现什么后果？**

答案是：不会发生任何结果，`QThread`还是继续执行它的`run`函数，`run`函数不会被重新调用。虽然在线程未结束时调用`start`不会出现什么结果，但为了谨慎起见，还是建议在start之前进行判断：
```cpp
void Widget::onButtonQThreadClicked()
{
    ui->progressBar->setValue(0);
    if(m_thread->isRunning())
    {
        return;
    }
    m_thread->start();
}
```
这种调用方法估计了解过QThread的都知道

- **在线程运行过程调用quit函数有什么效果**

答案是：不会发生任何效果，`QThread`不会因为你调用quit函数而退出正在运行到一半的run，正确退出线程的方法上面有介绍。那quit到底有什么用的呢，这要到下篇才能看出它的作用。使用moveToThread方法执行多线程时，这个函数将有大作用。

- **程序在退出时要判断各线程是否已经退出，没退出的应该让它终止**
如果不进行判断，很可能程序退出时会崩溃。如果线程的父对象是窗口对象，那么在窗体的析构函数中，还需要调用`wait`函数等待线程完全结束再进行下面的析构。

- **善用`QObject::deleteLater` 和 `QObject::destroyed`来进行内存管理**
由于多线程环境你不可预料下一步是哪个语句执行，因此，加锁和自动删除是很有用的工具，加锁是通过效率换取安全，用Qt的信号槽系统可以更有效的处理这些问题。


示例代:

[--> 见 github ](https://github.com/czyt1988/czyBlog/tree/master/tech/QtThread)

