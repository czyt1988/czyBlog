#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();
    void on_timeOut();
private:
    Ui::Widget *ui;
    QTimer m_timer;
    int m_present;
};

#endif // WIDGET_H
