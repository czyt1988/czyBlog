#ifndef DFIXEDTABLEWIDGET_H
#define DFIXEDTABLEWIDGET_H
#include "DTableWidgetEx.h"
///
/// \brief 固定表格，此表格无滚动条，但可以通过信号使得表格滚动
///
/// 表格默认15行，6列（不算两边表头），表头都比较大
///
class DFixedTableWidget : public DTableWidgetEx
{
    Q_OBJECT
public:
    DFixedTableWidget(QWidget* par);
    //设置水平header的标题，此标题会显示在左上角的偏右边
    void setHorizontalHeaderTitle(const QString& str);
    //设置垂直header的标题，此标题会显示在左上角的偏左边
    void setVerticalHeaderTitle(const QString& str);

protected:
    //重写边角斜线
    virtual void paintConnerButton(QStylePainter& painter,QAbstractButton* btn);
private:
    QString m_horizontalHeaderTitle;///< 水平header的标题，此标题会显示在左上角的偏右边
    QString m_verticalHeaderTitle;///< 垂直header的标题，此标题会显示在左上角的偏左边
};


#endif // DFIXEDTABLEWIDGET_H
