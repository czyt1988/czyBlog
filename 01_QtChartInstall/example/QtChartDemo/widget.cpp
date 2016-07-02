#include "widget.h"
#include "ui_widget.h"
#include <QLineSeries>
#include <QComboBox>
#include <math.h>
#include <QBarSeries>
#include <QBarSet>
#include <QTime>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
  ,m_chart(new QChart())
{
    ui->setupUi(this);
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
    ui->comboBox_theme->setCurrentIndex(-1);
    connect(ui->comboBox_theme,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
            ,this,&Widget::on_comboThemeIndexChanged);
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
    connect(ui->pushButton_addBar,&QAbstractButton::clicked,this,&Widget::on_pushButton_2axis_clicked);
    connect(ui->pushButton_clear,&QAbstractButton::clicked,this,&Widget::on_pushButton_clear);


    m_chart->setTitle(tr("简单的QtChart示例"));
}
Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_1Simple_clicked()
{
    static int s_line_count = 0;
    ++s_line_count;
    QLineSeries* line1 = new QLineSeries();
    const int size = 1000;
    std::vector<double> v1;
    randData(size,v1);
    for(int i=0;i<size;++i)
    {
        line1->append(double(i),v1[i]);
    }
    line1->setName( tr("随机数曲线") + QString::number(s_line_count) );
    m_chart->addSeries(line1);
    m_chart->createDefaultAxes();

}


    //ui->chart->setRenderHint(QPainter::Antialiasing);
void Widget::on_pushButton_2axis_clicked()
{

}



void Widget::on_pushButton1_noLegend_clicked()
{
    m_chart->legend()->hide();
}

void Widget::on_comboThemeIndexChanged(int index)
{
    if(!m_chart)
        return;
    if(index<0)
        return;
    QVariant var = ui->comboBox_theme->currentData();
    if(!var.isValid())
        return;
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(var.value<int>());
    m_chart->setTheme(theme);
    m_chart->update();
}

void Widget::on_pushButton_addBar()
{
    static int s_bar_count = 0;
    ++s_bar_count;
    QBarSeries* bar = new QBarSeries();
    std::vector<double> val;
    for(double x=0;x<10;++x)
    {
        val.clear();
        randData(3,val);
        QBarSet* setData = new QBarSet(QString("%1").arg(x));
        bar->append(setData);
    }
    bar->setName(tr("bar示例%1").arg(s_bar_count));
    m_chart->addSeries(bar);
    m_chart->createDefaultAxes();
}

void Widget::on_pushButton_clear()
{
    m_chart->removeAllSeries();
}

void Widget::randData(int size,std::vector<double>& data) const
{
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    int val;
    for(int i=0;i<size;++i)
    {
        val = qrand();
        data.push_back(double(val)/double(RAND_MAX));
    }
}
