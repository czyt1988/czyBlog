#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCharts>
#include <vector>
QT_CHARTS_USE_NAMESPACE
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
    void on_pushButton_2axis_clicked();

    void on_pushButton_1Simple_clicked();

    void on_pushButton1_noLegend_clicked();

    void on_comboThemeIndexChanged(int index);

    void on_pushButton_addBar();

    void on_pushButton_clear();
private:
    void randData(int size,std::vector<double>& data) const;
private:
    Ui::Widget *ui;
    QChart* m_chart;
};

#endif // WIDGET_H
