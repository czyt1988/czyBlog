#include "DTableWidgetEx.h"
#include <QHeaderView>
#include <QAbstractButton>
#include <QEvent>
#include <QStylePainter>
DTableWidgetEx::DTableWidgetEx(QWidget* par):QTableWidget(par)
{
    m_connerBtn = findChild<QAbstractButton*>();
    if (m_connerBtn)
    {
        m_connerBtn->setObjectName("QTableCornerButton");
        m_connerBtn->installEventFilter(this);
    }
}
///
/// \brief 获取顶部交点按钮的尺寸
/// \return
///
QSize DTableWidgetEx::getCornerButtonSize() const
{
    return QSize(verticalHeader()->width()-1,horizontalHeader()->height()-1);
}
///
/// \brief 绘制左上角按钮
/// \param painter 设置好绘制设备的painter
/// \param btn 左上角按钮,左上角按钮指针，Painter已经把此按钮设置为此绘制设备
///
void DTableWidgetEx::paintConnerButton(QStylePainter &painter,QAbstractButton* btn)
{
    QStyleOptionHeader opt;
    opt.init(btn);
    QStyle::State state = QStyle::State_None;
    if (btn->isEnabled())
        state |= QStyle::State_Enabled;
    if (btn->isActiveWindow())
        state |= QStyle::State_Active;
    if (btn->isDown())
        state |= QStyle::State_Sunken;
    opt.state = state;
    opt.rect = btn->rect();
    opt.text = btn->text(); // this line is the only difference to QTableCornerButton

    opt.position = QStyleOptionHeader::OnlyOneSection;
    painter.drawControl(QStyle::CE_Header, opt);
}

QAbstractButton *DTableWidgetEx::connerButton()
{
    return m_connerBtn;
}

const QAbstractButton *DTableWidgetEx::connerButton() const
{
    return m_connerBtn;
}

bool DTableWidgetEx::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Paint)
    {
        QAbstractButton* btn = qobject_cast<QAbstractButton*>(obj);
        if(btn == NULL)
        {
            return true;
        }
        if(btn != m_connerBtn)
        {
            return true;
        }
        QStylePainter painter(btn);
        paintConnerButton(painter,btn);
        return true; // eat event
    }
    return false;
}

