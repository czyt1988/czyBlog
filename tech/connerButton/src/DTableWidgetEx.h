#ifndef DTABLEWIDGETEX_H
#define DTABLEWIDGETEX_H
#include <QTableWidget>
#include <QStylePainter>
class QAbstractButton;
///
/// \brief 可以对QTableCornerButton进行重绘的表格窗体
///
/// 此表格和QTableWidget最大的不同在于可以对左上角的CornerButton进行重绘，例如要绘制文字或者绘制线条等动作
/// 同时可以获取QAbstractButton*的指针进行一些信号或槽绑定操作
/// \date 2016-08-19
/// \author czy
///
class DTableWidgetEx : public QTableWidget
{
    Q_OBJECT
public:
    DTableWidgetEx(QWidget* par = NULL);
    //获取顶部交点按钮的尺寸
    QSize getCornerButtonSize() const;
    virtual void paintConnerButton(QStylePainter& painter,QAbstractButton* btn);
    QAbstractButton* connerButton();
    const QAbstractButton *connerButton() const;
protected:
    bool eventFilter(QObject *obj, QEvent *event);
private:
    QAbstractButton* m_connerBtn;
};

#endif // DTABLEWIDGETEX_H
