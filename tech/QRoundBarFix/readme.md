#Qt第三方圆形进度条的改进
要实现一个圆形的进度条功能，在网上找到一个比较出名的第三方封装类：QRoundProgressBar，地址：[sourceforge 的 QRoundProgressBar](https://sourceforge.net/projects/qroundprogressbar/)
功能封装的还是不错，提供了3种模式，线形、圆环、饼状。使用过程中发现圆环进度条对背景透明支持不够完善，内圆的背景无法实现透明，为了解决此问题，下面对此控件进行了一些修订，实现完整的圆形进度条。

#QRoundProgressBar目前存在的不足

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QRoundBarFix/pic/0.png)

QRoundProgressBar在带背景图片widget下使用StyleDonut样式时，内环背景无法透明

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QRoundBarFix/pic/1.gif)

代码如下：
头文件:
```cpp
class DRoundProgressBar;
class QTimer;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
public slots:
    void onTimeOut();
private:
    Ui::Widget *ui;
    QTimer* mTimer;
    DRoundProgressBar* mRoundBar;
    int mPresent;
};
```
cpp:
```cpp
#include "Widget.h"
#include "ui_Widget.h"
#include "DRoundProgressBar.h"
#include <QTimer>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setAutoFillBackground(true);
    QPixmap img(":/img/BlueDialog_BK.png");
    QPalette bkPalette;
    bkPalette.setBrush(QPalette::Window,QBrush(img));
    setPalette(bkPalette);
    mRoundBar = new DRoundProgressBar(this);
    mRoundBar->setGeometry(150,100,500,500);
    mRoundBar->setBarStyle(DRoundProgressBar::StyleDonut);
    mRoundBar->setRange(0,100);
    QPalette palette;
    palette.setBrush(QPalette::Window,Qt::NoBrush);
    palette.setBrush(QPalette::AlternateBase,Qt::NoBrush);
    palette.setBrush(QPalette::Highlight,QBrush(QColor(0,140,255)));
    palette.setColor(QPalette::Text,QColor(0,0,0));
    //palette.setBrush(QPalette::Base,Qt::white);
    mRoundBar->setPalette(palette);
    mTimer = new QTimer(this);
    mTimer->setInterval(200);
    connect(mTimer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
    mPresent = 0;
    mTimer->start();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onTimeOut()
{
    ++mPresent;
    if(mPresent >= 100)
    {
        mPresent = 0;
    }
    mRoundBar->setValue(mPresent);
}
```

这里把QPalette::Window和QPalette::AlternateBase设置为透明，发现无法绘制圆环

## 原因
查看代码，看看其绘制圆环的步骤
```cpp
void QRoundProgressBar::drawValue(QPainter &p, const QRectF &baseRect, double value, double arcLength)
{
    // nothing to draw
    if (value == m_min)
        return;

    // for Line style
    if (m_barStyle == StyleLine)
    {
        p.setPen(QPen(palette().highlight().color(), m_dataPenWidth));
        p.setBrush(Qt::NoBrush);
        p.drawArc(baseRect.adjusted(m_outlinePenWidth/2, m_outlinePenWidth/2, -m_outlinePenWidth/2, -m_outlinePenWidth/2),
                  m_nullPosition * 16,
                  -arcLength * 16);
        return;
    }

    // for Pie and Donut styles

    QPainterPath dataPath;
    dataPath.setFillRule(Qt::WindingFill);

    // pie segment outer
    dataPath.moveTo(baseRect.center());
    dataPath.arcTo(baseRect, m_nullPosition, -arcLength);
    dataPath.lineTo(baseRect.center());

    p.setBrush(palette().highlight());
    p.setPen(QPen(palette().shadow().color(), m_dataPenWidth));
    p.drawPath(dataPath);
}
```
发现绘制圆环和绘制pie是一个代码，而实现圆环只不过是用一个背景覆盖上去了，这样，如果让中间区域透明就会显示画的那个扇形的那一部分(原来作者是得多懒-_-#)。
因此。在绘制圆环时需要特殊对待，`QPainterPath`在画圆环时把圆环填充。

## 改进

这里需要把原来的`drawValue`函数进行修正，原来`drawValue`函数对绘制pie和绘制Donut styles是一样处理，这里修正为pie就画pie，画Donut styles就画圆环：
为了绘制圆环，`drawValue`函数需添加两个变量，是内环的对应的矩形和半径
```cpp
virtual void drawValue(QPainter& p, const QRectF& baseRect, double value, double arcLength, const QRectF & innerRect, double innerRadius);
```
改动后的cpp
```cpp
void QRoundProgressBar::drawValue(QPainter &p
                                  , const QRectF &baseRect
                                  , double value
                                  , double arcLength
                                  , const QRectF & innerRect
                                  , double innerRadius)
{
    // nothing to draw
    if (value == m_min)
        return;

    // for Line style
    if (m_barStyle == StyleLine)
    {
        p.setPen(QPen(palette().highlight().color(), m_dataPenWidth));
        p.setBrush(Qt::NoBrush);
        p.drawArc(baseRect.adjusted(m_outlinePenWidth/2, m_outlinePenWidth/2, -m_outlinePenWidth/2, -m_outlinePenWidth/2),
                  m_nullPosition * 16,
                  -arcLength * 16);
        return;
    }

    // for Pie and Donut styles
    QPainterPath dataPath;
    dataPath.setFillRule(Qt::WindingFill);
    dataPath.moveTo(baseRect.center());
    dataPath.arcTo(baseRect, m_nullPosition, -arcLength);//大家都是先绘制外圆的弧长
    if(m_barStyle == StylePie)
    {

        // pie segment outer
        dataPath.lineTo(baseRect.center());

        p.setPen(QPen(palette().shadow().color(), m_dataPenWidth));
    }
    if(m_barStyle == StyleDonut)
    {
        // draw dount outer
        QPointF currentPoint = dataPath.currentPosition();//绘制完外圆弧长后，获取绘制完的位置绘制一个直线到达内圆
        currentPoint = baseRect.center() + ((currentPoint - baseRect.center()) * m_innerOuterRate;//计算内圆的坐标点，m_innerOuterRate替代了原作者写的0.75，代表内圆是外圆的0.75倍
        dataPath.lineTo(currentPoint);//绘制外圆到内圆的直线
        dataPath.moveTo(baseRect.center());//坐标点回到中心准备绘制内圆弧形
        dataPath.arcTo(innerRect, m_nullPosition-arcLength, arcLength);//绘制内圆的弧形
        currentPoint = dataPath.currentPosition();//准备绘制内圆到外圆的直线，形成封闭区域
        currentPoint = baseRect.center() + ((currentPoint - baseRect.center()) * (2-m_innerOuterRate));//绘制内圆到外圆的直线，这里2-m_innerOuterRate其实是对应(1 + (1 -m_innerOuterRate))的
        dataPath.lineTo(currentPoint);
        p.setPen(Qt::NoPen);//这个很重要不然就会有绘制过程的一些轮廓了
    }
    p.setBrush(palette().highlight());
    p.drawPath(dataPath);
}
```
具体过程见代码的注释。

这里作者把内圆直径定死为外圆的0.75倍，我觉得这样失去了灵活性，因此加入了一个float变量，m_innerOuterRate，默认为0.75，替代原来的0.75常数，并加入方法`float innerOuterRate() const`和`void setInnerOuterRate(float r)`进行设置

原来的`paintEvent`函数的函数顺序也需要改变：

下面是原来的paintEvent函数：
```cpp
void QRoundProgressBar::paintEvent(QPaintEvent* /*event*/)
{
    double outerRadius = qMin(width(), height());
    QRectF baseRect(1, 1, outerRadius-2, outerRadius-2);

    QImage buffer(outerRadius, outerRadius, QImage::Format_ARGB32_Premultiplied);

    QPainter p(&buffer);
    p.setRenderHint(QPainter::Antialiasing);

    // data brush
    rebuildDataBrushIfNeeded();

    // background
    drawBackground(p, buffer.rect());

    // base circle
    drawBase(p, baseRect);

    // data circle
    double arcStep = 360.0 / (m_max - m_min) * m_value;
    drawValue(p, baseRect, m_value, arcStep);

    // center circle
    double innerRadius(0);
    QRectF innerRect;
    calculateInnerRect(baseRect, outerRadius, innerRect, innerRadius);
    drawInnerBackground(p, innerRect);

    // text
    drawText(p, innerRect, innerRadius, m_value);

    // finally draw the bar
    p.end();

    QPainter painter(this);
    painter.fillRect(baseRect, palette().background());
    painter.drawImage(0,0, buffer);
}
```

原来作者使用了QImage作为缓存，但qt自带双缓冲，这一步没有必要(用QImage时在ubuntu还有小问题，在控件比较小时(200*200以下)会出现花屏现象，原因未知),修改为：

```cpp

void QRoundProgressBar::paintEvent(QPaintEvent* /*event*/)
{
    double outerRadius = qMin(width(), height());
    QRectF baseRect(1, 1, outerRadius-2, outerRadius-2);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // data brush
    rebuildDataBrushIfNeeded();

    // background
    drawBackground(p, rect());
    double innerRadius(0);
    QRectF innerRect;
    calculateInnerRect(baseRect, outerRadius, innerRect, innerRadius);
    double arcStep = 360.0 / (m_max - m_min) * m_value;
    // base circle
    drawBase(p, baseRect,innerRect);

    // data circle

    drawValue(p, baseRect, m_value, arcStep,innerRect, innerRadius);

    // center circle

    drawInnerBackground(p, innerRect);

    // text
    drawText(p, innerRect, innerRadius, m_value);

    // finally draw the bar
    p.end();
}
```

主要是把`calculateInnerRect(baseRect, outerRadius, innerRect, innerRadius);`函数提前计算出内圆对应的参数。并传入给新修改的`drawValue`函数。把多余的双缓冲机制去掉.

此时效果还未达到需求的效果，发现`drawBase`函数还需要修改，原来的`drawBase`函数如下：

```cpp
void QRoundProgressBar::drawBase(QPainter &p, const QRectF &baseRect)
{
    switch (m_barStyle)
    {
    case StyleDonut:
        p.setPen(QPen(palette().shadow().color(), m_outlinePenWidth));
        p.setBrush(palette().base());
        p.drawEllipse(baseRect);
        break;

    case StylePie:
        p.setPen(QPen(palette().base().color(), m_outlinePenWidth));
        p.setBrush(palette().base());
        p.drawEllipse(baseRect);
        break;

    case StyleLine:
        p.setPen(QPen(palette().base().color(), m_outlinePenWidth));
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(baseRect.adjusted(m_outlinePenWidth/2, m_outlinePenWidth/2, -m_outlinePenWidth/2, -m_outlinePenWidth/2));
        break;

    default:;
    }
}
```

上面的`drawBase`函数可见，由于原作者比较懒，对于Donut styles模式就直接画了一个外圆，并不是一个空心圆环，改进如下：

```cpp
void QRoundProgressBar::drawBase(QPainter &p, const QRectF &baseRect,const QRectF &innerRect)
{
    switch (m_barStyle)
    {
    case StyleDonut:
    {
        QPainterPath dataPath;
        dataPath.setFillRule(Qt::OddEvenFill);
        dataPath.moveTo(baseRect.center());
        dataPath.addEllipse(baseRect);
        dataPath.addEllipse(innerRect);
        p.setPen(QPen(palette().shadow().color(), m_outlinePenWidth));
        p.setBrush(palette().base());
        p.drawPath(dataPath);
        break;
    }
    case StylePie:
        p.setPen(QPen(palette().base().color(), m_outlinePenWidth));
        p.setBrush(palette().base());
        p.drawEllipse(baseRect);
        break;

    case StyleLine:
        p.setPen(QPen(palette().base().color(), m_outlinePenWidth));
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(baseRect.adjusted(m_outlinePenWidth/2, m_outlinePenWidth/2, -m_outlinePenWidth/2, -m_outlinePenWidth/2));
        break;
    default:;
    }
}
```
最后运行效果：

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QRoundBarFix/pic/2.gif)

代码见:[czyt1988的github](https://github.com/czyt1988/czyBlog/tree/master/tech/QRoundBarFix)











