#include "DFixedTableWidget.h"
#include <QHeaderView>
#include <QPainter>
#include <QAbstractButton>
#include <QDebug>
#include <QEvent>

DFixedTableWidget::DFixedTableWidget(QWidget *par):DTableWidgetEx(par)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setRowCount(15);
    setColumnCount(6);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    QHeaderView* header = verticalHeader();
    header->setDefaultAlignment(Qt::AlignCenter);
    header->setDefaultSectionSize(32);//竖向尺寸43
    header->setHighlightSections(false);// 禁止选中高亮
    header->setResizeMode(QHeaderView::Fixed);//禁止可调
    header->setClickable(false);// 禁止可点击
    header->setMovable(false);// 禁止移动
    header->setFixedWidth(125);//固定宽度
    header->setStyleSheet(QString(
                   "QHeaderView::section{"
                   "background-color: rgb(255, 255, 255);"
                   //"border: none;"
                              "border-style: none;"
                   "border-right: 1px solid #4CB5FF;"
                  // "border-left: 1px solid #4CB5FF;"
                   "border-bottom: 1px solid #4CB5FF;"
                      "}"
                      ));

    header = horizontalHeader();
    header->setDefaultAlignment(Qt::AlignCenter);
    header->setDefaultSectionSize(100);//尺寸100
    header->setHighlightSections(false);// 禁止选中高亮
    header->setResizeMode(QHeaderView::Fixed);//禁止可调
    header->setClickable(false);// 禁止可点击
    header->setMovable(false);// 禁止移动
    header->setFixedHeight(43);//固定高度
    header->setStyleSheet(QString(
                   "QHeaderView::section{"
                   "background-color: rgb(255, 255, 255);"
                   "border-style: none;"
                   "border-right: 1px solid #4CB5FF;"
                    "border-bottom: 1px solid #4CB5FF;"
                      "}"
                      ));
    setStyleSheet(QString(
                   "QTableView{"
                   "background-color: rgb(255, 255, 255);"
                   "gridline-color: rgb(76,181,255);"
                   "border: 1px solid #4CB5FF;"
                   "}"

//                   "QTableView QTableCornerButton::section{"
//                   "background-color: rgb(255, 255, 255);"
//                   //"border-right: 1px solid #4CB5FF;"
//                   //"border-bottom: 1px solid #4CB5FF;"
//                   "border: 1px solid #4CB5FF;"
//                   "}"
                      ));
}

void DFixedTableWidget::setHorizontalHeaderTitle(const QString &str)
{
    m_horizontalHeaderTitle = str;
}

void DFixedTableWidget::setVerticalHeaderTitle(const QString &str)
{
    m_verticalHeaderTitle = str;
}

void DFixedTableWidget::paintConnerButton(QStylePainter &painter, QAbstractButton *btn)
{
    Q_UNUSED(btn);
    QSize crs = getCornerButtonSize();
    QPen pen;
    pen.setStyle(Qt::SolidLine);

    //设置文字颜色
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawText(62,-2,58,23,Qt::AlignLeft,m_horizontalHeaderTitle);
    painter.drawText(3,17,50,23,Qt::AlignLeft,m_verticalHeaderTitle);

    pen.setColor(QColor(76,181,255));
    painter.setPen(pen);
    //绘制表格左上角交接斜线
    painter.drawLine(0,0,crs.width(),crs.height());
    //绘制表格左上角交接的下边线
    painter.drawLine(0,crs.height(),crs.width(),crs.height());
    //绘制表格左上角交接的右边边线
    painter.drawLine(crs.width(),0,crs.width(),crs.height());

}



