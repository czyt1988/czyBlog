#include "widget.h"
#include "ui_widget.h"
#include <QLineSeries>
#include <math.h>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_chart = new QChart();
    ui->chart->setChart(m_chart);
    ui->checkBox_legend->setChecked(true);
    ui->comboBox_theme->addItem("Light", QChart::ChartThemeLight);
    ui->comboBox_theme->addItem("Blue Cerulean", QChart::ChartThemeBlueCerulean);
    ui->comboBox_theme->addItem("Dark", QChart::ChartThemeDark);
    ui->comboBox_theme->addItem("Brown Sand", QChart::ChartThemeBrownSand);
    ui->comboBox_theme->addItem("Blue NCS", QChart::ChartThemeBlueNcs);
    ui->comboBox_theme->addItem("High Contrast", QChart::ChartThemeHighContrast);
    ui->comboBox_theme->addItem("Blue Icy", QChart::ChartThemeBlueIcy);
    ui->comboBox_theme->addItem("Qt", QChart::ChartThemeQt);
    connect(ui->checkBox_legend,&QAbstractButton::clicked,this,[&](bool v){
        m_chart->legend()->setVisible(v);
    });
    connect(ui->pushButton_legendTop,&QAbstractButton::clicked,this,[&](){
        m_chart->legend()->setAlignment(Qt::AlignTop);
    });
    connect(ui->pushButton_legendBottom,&QAbstractButton::clicked,this,[&](){
        m_chart->legend()->setAlignment(Qt::AlignBottom);
    });
    connect(ui->pushButton_legendLeft,&QAbstractButton::clicked,this,[&](){
        m_chart->legend()->setAlignment(Qt::AlignLeft);
    });
    connect(ui->pushButton_legendRight,&QAbstractButton::clicked,this,[&](){
        m_chart->legend()->setAlignment(Qt::AlignRight);
    });
}
Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_1Simple_clicked()
{
    m_chart->removeAllSeries();
    QLineSeries* line1 = new QLineSeries();
    QLineSeries* line2 = new QLineSeries();
    for(double x=0;x<10;x+=0.1)
    {
        line1->append(x,sin(x));
        line2->append(x*10,cos(x*10));
    }
    line1->setName(QStringLiteral("我是一个正弦函数[0~10]"));
    line2->setName(QStringLiteral("我是一个余弦函数[0~100]"));
    m_chart->addSeries(line1);
    m_chart->addSeries(line2);
    m_chart->createDefaultAxes();
    m_chart->setTitle(QStringLiteral("简单的QtChart示例"));
}


    //ui->chart->setRenderHint(QPainter::Antialiasing);
void Widget::on_pushButton_2axis_clicked()
{

}



void Widget::on_pushButton1_noLegend_clicked()
{
    m_chart->legend()->hide();
}
