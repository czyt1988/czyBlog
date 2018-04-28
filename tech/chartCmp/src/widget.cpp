#include "widget.h"
#include "ui_widget.h"
#include <QChart>
#include <QValueAxis>
#include <QLineSeries>
#include <math.h>
#include <QElapsedTimer>
#include <QDebug>
#include <math.h>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initQtChart();
    ui->splitterTop->setStretchFactor(0,3);
    ui->splitterTop->setStretchFactor(1,1);
    ui->splitterQtChart->setStretchFactor(0,1);
    ui->splitterQtChart->setStretchFactor(1,3);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::plotQtChart()
{
    QChart* chart = m_qtChart;
    QValueAxis * axisX = qobject_cast<QValueAxis *>(chart->axisX());
    QValueAxis * axisY = qobject_cast<QValueAxis *>(chart->axisY());
    QLineSeries* line = new QLineSeries;
    line->setUseOpenGL(true);
    QVector<QPointF> points;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i=0;i<320000;++i)
    {
        points.append(QPointF(i,qrand()));
    }
    line->replace(points);
    chart->addSeries(line);
    line->attachAxis(axisX);
    line->attachAxis(axisY);
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

    m_qtChart = chart;
}

void Widget::on_pushButton_plot_clicked()
{
    QElapsedTimer timer;
    timer.start();
    plotQtChart();
    qDebug()<<timer.elapsed();
}



void Widget::on_checkBoxQtChartSetAnimation_clicked(bool checked)
{
    m_qtChart->setAnimationOptions(checked ? QChart::AllAnimations : QChart::NoAnimation);
}
