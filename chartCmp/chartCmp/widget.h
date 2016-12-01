#ifndef WIDGET_H
#define WIDGET_H
#include <QtCharts>
using namespace QtCharts;
#include <QWidget>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void plotQtChart();
    void initQtChart();
private slots:
    void on_pushButton_plot_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
