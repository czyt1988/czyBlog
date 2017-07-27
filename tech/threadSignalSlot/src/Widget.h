#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>
namespace Ui {
class Widget;
}

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
    mutable bool m_abort;
};

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

#endif // WIDGET_H
