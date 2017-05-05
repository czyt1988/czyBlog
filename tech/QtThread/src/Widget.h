#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
class ThreadFromQThread;
class ThreadObject;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
signals:
    void startObjThreadWork1();
    void startObjThreadWork2();
private slots:
    void onButtonQThreadClicked();
    void onButtonQthread1SetSomethingClicked();
    void onButtonQthread1GetSomethingClicked();
    void onButtonQthreadQuitClicked();
    void onButtonQthreadTerminateClicked();
    void onButtonQThreadDoSomthingClicked();
    void onButtonQThreadExitClicked();
    void onButtonQThreadRunLoaclClicked();

    void onQThreadFinished();
    void onLocalThreadDestroy(QObject* obj);

    void onButtonObjectMove2ThreadClicked();
    void onButtonObjectMove2Thread2Clicked();
    void onButtonObjectThreadStopClicked();
    void startObjThread();



    void progress(int val);
    void receiveMessage(const QString& str);
    void heartTimeOut();


private:
    Ui::Widget *ui;
    ThreadFromQThread* m_thread;
    QTimer m_heart;
    ThreadObject* m_obj;
    QThread* m_objThread;
    ThreadFromQThread* m_currentRunLoaclThread;
};

#endif // WIDGET_H
