#include "widget.h"
#include "ui_widget.h"
#include <QChart>
#include <QValueAxis>
#include <QLineSeries>
#include <math.h>
#include <QElapsedTimer>
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initQtChart();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::plotQtChart()
{
    QChart* chart = ui->qtChart->chart();
    if(!chart)
    {
        chart = new QChart();
        ui->qtChart->setChart(chart);
    }
    QValueAxis * axisX = qobject_cast<QValueAxis *>(chart->axisX());
    QValueAxis * axisY = qobject_cast<QValueAxis *>(chart->axisY());
    QLineSeries* line = NULL;
    if(chart->series().size()>0)
    {
        line = qobject_cast<QLineSeries*>(chart->series().at(0));
    }
    else
    {
        line = new QLineSeries;
    }
    line->setUseOpenGL(true);
    chart->addSeries(line);
    line->attachAxis(axisX);
    line->attachAxis(axisY);

    QVector<QPointF> points;
    for(int i=0;i<320000;++i)
    {
        points.append(QPointF(i,sin(i)));
    }
    line->replace(points);
}

void Widget::initQtChart()
{
    QChart* chart = ui->qtChart->chart();
    if(!chart)
    {
        chart = new QChart();
        ui->qtChart->setChart(chart);
    }
    QValueAxis *axisX = new QValueAxis;
    QValueAxis *axisY = new QValueAxis;
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->legend()->hide();
}

void Widget::on_pushButton_plot_clicked()
{
    QElapsedTimer timer;
    timer.start();
    plotQtChart();
    qDebug()<<timer.elapsed();
}
