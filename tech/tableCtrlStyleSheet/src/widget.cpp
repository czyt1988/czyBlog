#include "widget.h"
#include "ui_widget.h"
#include <QScrollBar>
#include <QScrollArea>
#include <QHeaderView>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setRowCount(15);
    ui->tableWidget->setColumnCount(6);
    QHeaderView* header = ui->tableWidget->verticalHeader();
    header->setDefaultAlignment(Qt::AlignCenter);
    header->setDefaultSectionSize(32);//竖向尺寸32
    header->setHighlightSections(false);// 禁止选中高亮
    header->setSectionResizeMode(QHeaderView::Fixed);//禁止可调
    header->setSectionsClickable(false);// 禁止可点击
    header->setSectionsMovable(false);// 禁止移动
    header->setFixedWidth(128);//固定宽度
    header->setStyleSheet(QString(
                       "QHeaderView::section{"
                       "background-color: rgb(255, 255, 255);"
                       "border: none;"
                       "border-right: 1px solid #4CB5FF;"
                      // "border-left: 1px solid #4CB5FF;"
                       "border-bottom: 1px solid #4CB5FF;"
                          "}"
                          ));

    header = ui->tableWidget->horizontalHeader();
    header->setDefaultAlignment(Qt::AlignCenter);
    header->setDefaultSectionSize(128);//竖向尺寸32
    header->setHighlightSections(false);// 禁止选中高亮
    header->setSectionResizeMode(QHeaderView::Fixed);//禁止可调
    header->setSectionsClickable(false);// 禁止可点击
    header->setSectionsMovable(false);// 禁止移动
    header->setFixedHeight(32);//固定高度
    header->setStyleSheet(QString(
                       "QHeaderView::section{"
                       "background-color: rgb(255, 255, 255);"
                       "border: none;"
                       "border-right: 1px solid #4CB5FF;"
                        "border-bottom: 1px solid #4CB5FF;"
                          "}"
                          ));
    ui->tableWidget->setStyleSheet(QString(
                                       "QTableView{"
                                       "background-color: rgb(255, 255, 255);"
                                       "gridline-color: rgb(76,181,255);"
                                       "border: 1px solid #4CB5FF;"
                                       "}"

                                       "QTableView QTableCornerButton::section{"
                                       "background-color: rgb(255, 255, 255);"
                                       "border-right: 1px solid #4CB5FF;"
                                       "border-bottom: 1px solid #4CB5FF;"
                                       "}"
                                       ));


}

Widget::~Widget()
{
    delete ui;
}
