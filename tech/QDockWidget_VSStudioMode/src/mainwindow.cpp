#include "mainwindow.h"
#include "ui_mainwindow.h"
#define APPEND_DOCK(d)\


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //删除中央窗体
    QWidget* p = takeCentralWidget();
    if(p)
        delete p;
    //允许嵌套dock
    setDockNestingEnabled(true);
    //记录所有的dock指针
    m_docks.append(ui->dockWidget_1);
    m_docks.append(ui->dockWidget_2);
    m_docks.append(ui->dockWidget_3);
    m_docks.append(ui->dockWidget_4);
    m_docks.append(ui->dockWidget_5);
    m_docks.append(ui->dockWidget_6);
    m_docks.append(ui->dockWidget_7);
    m_docks.append(ui->dockWidget_8);
    m_docks.append(ui->dockWidget_9);
}

MainWindow::~MainWindow()
{
    delete ui;
}
///
/// \brief 移除并隐藏所有的dock
///
void MainWindow::removeAllDock()
{
    for(int i=0;i<9;++i)
    {
        removeDockWidget(m_docks[i]);
    }
}
///
/// \brief 显示指定序号的dock
/// \param index 指定序号，如果不指定，则会显示所有
///
void MainWindow::showDock(const QList<int> &index)
{
    if (index.isEmpty())
    {
        for(int i=0;i<9;++i)
        {
            m_docks[i]->show();
        }
    }
    else
    {
        foreach (int i, index) {
            m_docks[i]->show();
        }
    }
}

void MainWindow::on_action_removeAll_triggered()
{
    removeAllDock();
}

void MainWindow::on_action_splitDockFun_triggered()
{
    removeAllDock();
    //原则，先左右，再上下
    addDockWidget(Qt::LeftDockWidgetArea,ui->dockWidget_1);
    splitDockWidget(ui->dockWidget_1,ui->dockWidget_2,Qt::Vertical);
    splitDockWidget(ui->dockWidget_2,ui->dockWidget_3,Qt::Vertical);
    splitDockWidget(ui->dockWidget_3,ui->dockWidget_4,Qt::Vertical);
    splitDockWidget(ui->dockWidget_4,ui->dockWidget_5,Qt::Vertical);
    showDock(QList<int>()<< 0<<1<<2<<3<<4);
}
void MainWindow::on_action_tabifyDockWidget_triggered()
{
removeAllDock();
//原则，先左右，再上下
addDockWidget(Qt::LeftDockWidgetArea,ui->dockWidget_1);
splitDockWidget(ui->dockWidget_1,ui->dockWidget_2,Qt::Horizontal);
splitDockWidget(ui->dockWidget_2,ui->dockWidget_3,Qt::Horizontal);
splitDockWidget(ui->dockWidget_1,ui->dockWidget_4,Qt::Vertical);
splitDockWidget(ui->dockWidget_2,ui->dockWidget_5,Qt::Vertical);
splitDockWidget(ui->dockWidget_3,ui->dockWidget_6,Qt::Vertical);
tabifyDockWidget(ui->dockWidget_1,ui->dockWidget_7);
tabifyDockWidget(ui->dockWidget_5,ui->dockWidget_8);
tabifyDockWidget(ui->dockWidget_3,ui->dockWidget_9);
showDock();
}
void MainWindow::on_action_addDockWidget_triggered()
{
    removeAllDock();
    //原则，先左右，再上下
    addDockWidget(Qt::LeftDockWidgetArea,ui->dockWidget_1);
    addDockWidget(Qt::RightDockWidgetArea,ui->dockWidget_2);
    addDockWidget(Qt::TopDockWidgetArea,ui->dockWidget_3);
    addDockWidget(Qt::BottomDockWidgetArea,ui->dockWidget_4);
    showDock(QList<int>()<< 0<<1<<2<<3);
}

void MainWindow::on_action_dock1_triggered()
{
    removeAllDock();
    //原则，先左右，再上下
    addDockWidget(Qt::LeftDockWidgetArea,ui->dockWidget_1);
    splitDockWidget(ui->dockWidget_1,ui->dockWidget_2,Qt::Horizontal);
    splitDockWidget(ui->dockWidget_2,ui->dockWidget_3,Qt::Horizontal);
    splitDockWidget(ui->dockWidget_1,ui->dockWidget_4,Qt::Vertical);
    splitDockWidget(ui->dockWidget_2,ui->dockWidget_5,Qt::Vertical);
    splitDockWidget(ui->dockWidget_3,ui->dockWidget_6,Qt::Vertical);
    splitDockWidget(ui->dockWidget_4,ui->dockWidget_7,Qt::Vertical);
    splitDockWidget(ui->dockWidget_5,ui->dockWidget_8,Qt::Vertical);
    splitDockWidget(ui->dockWidget_6,ui->dockWidget_9,Qt::Vertical);
    showDock();
}

void MainWindow::on_action_dock2_triggered()
{
    removeAllDock();
    //原则，先左右，再上下
    addDockWidget(Qt::LeftDockWidgetArea,ui->dockWidget_1);
    splitDockWidget(ui->dockWidget_1,ui->dockWidget_2,Qt::Horizontal);
    splitDockWidget(ui->dockWidget_2,ui->dockWidget_3,Qt::Horizontal);
    splitDockWidget(ui->dockWidget_1,ui->dockWidget_4,Qt::Vertical);
    splitDockWidget(ui->dockWidget_3,ui->dockWidget_6,Qt::Vertical);
    splitDockWidget(ui->dockWidget_4,ui->dockWidget_7,Qt::Vertical);
    splitDockWidget(ui->dockWidget_6,ui->dockWidget_9,Qt::Vertical);
    showDock(QList<int>()<< 0<<1<<2<<3<<5<<6<<8);
}

void MainWindow::on_action_dock3_triggered()
{
removeAllDock();
//原则，先左右，再上下
addDockWidget(Qt::TopDockWidgetArea,ui->dockWidget_1);
addDockWidget(Qt::LeftDockWidgetArea,ui->dockWidget_2);
splitDockWidget(ui->dockWidget_2,ui->dockWidget_3,Qt::Horizontal);
splitDockWidget(ui->dockWidget_3,ui->dockWidget_4,Qt::Horizontal);
splitDockWidget(ui->dockWidget_2,ui->dockWidget_5,Qt::Horizontal);
splitDockWidget(ui->dockWidget_3,ui->dockWidget_6,Qt::Horizontal);
splitDockWidget(ui->dockWidget_4,ui->dockWidget_7,Qt::Horizontal);
showDock(QList<int>()<< 0<<1<<2<<3<<4<<5<<6);
}


