#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QElapsedTimer>
#include <QDebug>
#include "QVariantListListModel.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  ,m_xls(NULL)
{
    ui->setupUi(this);
    QVariantListListModel* md = new QVariantListListModel(this);
    ui->tableView->setModel(md);
    md->setVariantListListPtr(&m_datas);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_open_triggered()
{
    QString xlsFile = QFileDialog::getOpenFileName(this,QString(),QString(),"excel(*.xls *.xlsx)");
    if(xlsFile.isEmpty())
        return;
    QElapsedTimer timer;
    timer.start();
    if(m_xls.isNull())
        m_xls.reset(new ExcelBase);
    m_xls->open(xlsFile);
    qDebug()<<"open cost:"<<timer.elapsed()<<"ms";timer.restart();
    m_xls->setCurrentSheet(1);
    m_xls->readAll(m_datas);
    qDebug()<<"read data cost:"<<timer.elapsed()<<"ms";timer.restart();
    QVariantListListModel* md = qobject_cast<QVariantListListModel*>(ui->tableView->model());
    if(md)
    {
        md->updateData();
    }
    qDebug()<<"show data cost:"<<timer.elapsed()<<"ms";timer.restart();
}

void MainWindow::on_action_write_triggered()
{
    QString xlsFile = QFileDialog::getExistingDirectory(this);
    if(xlsFile.isEmpty())
        return;
    xlsFile += "/excelRWByCztr1988.xls";
    QElapsedTimer timer;
    timer.start();
    if(m_xls.isNull())
        m_xls.reset(new ExcelBase);
    m_xls->create(xlsFile);
    qDebug()<<"create cost:"<<timer.elapsed()<<"ms";timer.restart();
    QList< QList<QVariant> > m_datas;
    for(int i=0;i<1000;++i)
    {
        QList<QVariant> rows;
        for(int j=0;j<100;++j)
        {
            rows.append(i*j);
        }
        m_datas.append(rows);
    }
    m_xls->setCurrentSheet(1);
    timer.restart();
    m_xls->writeCurrentSheet(m_datas);
    qDebug()<<"write cost:"<<timer.elapsed()<<"ms";timer.restart();
    m_xls->save();
}
