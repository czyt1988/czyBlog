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


    void on_checkBoxQtChartSetAnimation_clicked(bool checked);

private:
    Ui::Widget *ui;
    QChart* m_qtChart;
};

#endif // WIDGET_H
