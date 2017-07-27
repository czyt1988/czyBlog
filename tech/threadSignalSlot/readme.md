# Qt多线程信号和槽的常引用传递

 Qt经常用信号和槽传递变量，为了降低拷贝的消耗，传递变量时经常使用常引用(`const &`)作为变量的传递方式
 正常情况下，qt的信号槽都是在一个线程里执行，但在多线程的信号和槽传递时，使用常引用会不会出现意外情况呢？

如:

 ```cpp
 class TestThread : public QThread
 {
     Q_OBJECT
 public:
     TestThread(QObject* par);
     ~TestThread();
     void run();
 signals:
     void signalData(const int& a,unsigned int aAddress);
private:
     bool m_abort;
 };
```

 这个线程类有信号`signalData`,这个信号第一个参数通过引用传递

主要函数`run`的实现如下：

``` cpp
void TestThread::run()
{
    while(!m_abort)
    {
        if(1)
        {
            int a = 1;
            unsigned int add = (unsigned int)&a;
            emit signalData(a,add);
        }
        sleep(1);
        m_abort = true;
    }
}
```

`run`函数一直发出一个信号，有个临时变量`int a = 1;`信号`signalData`第一个参数是这个临时变量`a`的常引用
信号第二个参数是这个临时变量的地址。

接收端如果以引用方式接收a变量的话，很可能在接收到这个变量后，线程继续运行导致a已经析构了。

实际测试发现，接收端接收到的变量地址与发射端是不一样的，这是由于Qt连接信号槽的第5个参数的影响

连接信号槽第五个参数是`Qt::ConnectionType`,默认为自动判断，主要有`Qt::DirectConnection`和`Qt::QueuedConnection`着两种连接方式
自动判断模式下会根据连接的两个对象的线程id进行判断，如果线程id一样，使用`Qt::DirectConnection`直接连接，如果线程id不一样，会使用`Qt::QueuedConnection`方式队列连接

直接连接信号和槽和函数调用很像，类似于函数指针的调用，为了数据安全，多线程不会使用直接连接方式，当发射信号时，马上就调用连接的槽函数，顺序执行，再执行完槽函数后再继续执行下一步

队列连接
接收端代码：
``` cpp

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
signals:
    void signalData(const int& a,unsigned int aAddress);
private slots:
    void on_pushButton_test_clicked();
    void recDataQueuedConnection(const int& a,unsigned int aAddress);
    void recDataDirectConnection(const int& a,unsigned int aAddress);
    void recThreadData(const int& a,unsigned int aAddress);
private:
    Ui::Widget *ui;
    TestThread* m_thread;
};
···

cpp文件：

```cpp

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_thread = new TestThread(this);
    connect(this,SIGNAL(signalData(int,unsigned int))
            ,SLOT(recDataQueuedConnection(int,unsigned int))
            ,Qt::QueuedConnection
            );
    connect(this,SIGNAL(signalData(int,unsigned int))
            ,SLOT(recDataDirectConnection(int,unsigned int))
            ,Qt::DirectConnection
            );
    connect(m_thread,SIGNAL(signalData(int,unsigned int))
            ,SLOT(recThreadData(int,unsigned int))
            );
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_test_clicked()
{
    if(!m_thread->isRunning())
    {
        m_thread->start();
    }
    int a = 1;
    unsigned int add = (unsigned int)&a;

    ui->textBrowser->append("start emit local signal");
    emit signalData(a,add);
    ui->textBrowser->append("after emit local signal");
}

void Widget::recDataDirectConnection(const int &a, unsigned int aAddress)
{
    if(a != 1)
    {
        ui->textBrowser->append("a != 1");
    }
    if(aAddress != (unsigned int)&a)
    {
        ui->textBrowser->append(QString("DirectConnection addr not same,rec a:%1,thread a:%2").arg((unsigned int)&a).arg(aAddress));
    }
    else
    {
        ui->textBrowser->append(QString("DirectConnection addr is same,rec a:%1,thread a:%2").arg((unsigned int)&a).arg(aAddress));
    }
}

void Widget::recThreadData(const int &a, unsigned int aAddress)
{
    if(a != 1)
    {
        ui->textBrowser->append("a != 1");
    }
    if(aAddress != (unsigned int)&a)
    {
        ui->textBrowser->append(QString("[Thread]addr not same,rec a:%1,thread a:%2").arg((unsigned int)&a).arg(aAddress));
    }
    else
    {
        ui->textBrowser->append(QString("[Thread]addr is same,rec a:%1,thread a:%2").arg((unsigned int)&a).arg(aAddress));
    }

}

void Widget::recDataQueuedConnection(const int &a, unsigned int aAddress)
{
    if(a != 1)
    {
        ui->textBrowser->append("a != 1");
    }
    if(aAddress != (unsigned int)&a)
    {
        ui->textBrowser->append(QString("QueuedConnection addr not same,rec a:%1,thread a:%2").arg((unsigned int)&a).arg(aAddress));
    }
    else
    {
        ui->textBrowser->append(QString("QueuedConnection addr is same,rec a:%1,thread a:%2").arg((unsigned int)&a).arg(aAddress));
    }
}
```

这个测试建立了3个槽函数

```cpp
void recDataQueuedConnection(const int& a,unsigned int aAddress);
void recDataDirectConnection(const int& a,unsigned int aAddress);
void recThreadData(const int& a,unsigned int aAddress);
```

- 函数`recDataQueuedConnection`用于接收使用QueuedConnection连接的信号
函数`recDataDirectConnection`用于接收使用DirectConnection连接的信号(单线程下默认的连接方式)
函数`recThreadData`用于接收来自线程发过来的信号

构造函数显示了三个槽的连接方式

主要看函数`on_pushButton_test_clicked`

按钮点击触发这个槽，这个按钮进行如下动作：
- 开启线程(如果没跑的话)
发射一个`signalData`信号

结果如下：
```
start emit local signal
DirectConnection addr is same,rec a:2278944,thread a:2278944
after emit local signal
[Thread]addr not same,rec a:367921760,thread a:410320680
QueuedConnection addr not same,rec a:367830464,thread a:2278944
```
DirectConnection连接方式就和函数调用差不多，发射信号马上执行
QueuedConnection方式则不然，执行顺序看消息队列的排队情况，因此，传递的变量如果是引用的话会退化为传值，可以看到线程的信号或自身强制用QueuedConnection连接的信号，它传递到槽函数的常引用变量和发射时的常引用变量地址是不一样的

因此在Qt多线程中使用信号槽传递变量是非常安全的，不需要对变量进行加锁保护

