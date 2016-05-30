#简介

Qt的线性绘图控件有大名鼎鼎的[Qwt](http://qwt.sourceforge.net/)，[ChartDirector](http://www.advsofteng.com/cdcpp.html)，小巧玲珑的[QCustomPlot](http://www.qcustomplot.com/)，当然还有自家的[QtChart](http://doc.qt.io/QtCharts/index.html)。长久以来[QtChart](http://doc.qt.io/QtCharts/index.html)在Qt家族里一直是收费的模块，只有商业版才可以使用，但Qt5.7之后将开放其权限，可参见：[Qt 5.7 亮瞎眼的更新](http://blog.csdn.net/czyt1988/article/details/51351495)。下面将介绍如何安装QtChart并进行简单的绘图。

#下载
这里用git下载QtChart，参考[Qt Charts 5.7.0 安装教程](http://www.jianshu.com/p/8d9a817aa6b9)
如果大家还用Qt5.4以下版本请升级完再看，Qt5.4以下是不支持滴
Git地址：https://github.com/qtproject/qtcharts.git

```git
git clone https://github.com/qtproject/qtcharts.git
```

![这里写图片描述](http://img.blog.csdn.net/20160513002421712)

下载完目录是这样的：

![这里写图片描述](http://img.blog.csdn.net/20160513002539416)
开始编译前需要下载Perl：[activePerl](http://www.activestate.com/activeperl/downloads)

直接下一步点到头安装。
安装完成后需要重启一下电脑或者手动激活一下环境变量。

#编译QtChart

此时用Qt Creator加载qtcharts.pro

![加载后Qt Creator界面](http://img.blog.csdn.net/20160513223910568)

一杯茶的时间后：

![编译成功后界面](http://img.blog.csdn.net/20160513224820665)

此时在生成目录下生成：

![生成文件](http://img.blog.csdn.net/20160513224938509)

#安装

编译结束后就可以进行安装了
选择Qt Creator项目栏 - 运行标签 - 添加部署构建

![这里写图片描述](http://img.blog.csdn.net/20160514103724425)

这里需要添加两个部署，一个是QtChart的安装，一个是QtChart文档的编译。
如图所示配置完后，点击运行

![这里写图片描述](http://img.blog.csdn.net/20160514104251255)

这时会有很多复制的信息，它会把编译好的dll和头文件复制到Qt的安装目录下

![这里写图片描述](http://img.blog.csdn.net/20160514104609819)

![这里写图片描述](http://img.blog.csdn.net/20160514104625002)

打开Qt目录下的lib文件夹和include文件夹可以看到被复制过来的QtChart信息。
这时，再看QtChart的生成目录，比第一次编译时多了一个doc文件夹
你只要把这个doc文件夹里的qtcharts文件夹和qtcharts.qch文件复制到Qt安装目录下的Docs文件夹内对应版本文件夹下即可

![这里写图片描述](http://img.blog.csdn.net/20160514111423023)

这时打开Qt Assisant，搜QtChart，即可看到说明文档！

![这里写图片描述](http://img.blog.csdn.net/20160514111455430)

这时你的QtChart大部分已经配置完成。
还有example的安装，可参看文献[3]

#测试QtChart

下面演示如何用Qt Creator的界面设计师创建一个QtChart，其他方法可见参考文献。
首先新建一个Widget项目
使用一个Widget作为Chart的载体，在界面上放置一个Widget

![这里写图片描述](http://img.blog.csdn.net/20160514212657632)
右键点击这个widget，把这个Widget提升。

![这里写图片描述](http://img.blog.csdn.net/20160514212802227)
把这个Widget提升为QChartView

QtChart有两类绘图窗口容器，分别为QChart(QPolarChart)和QChartView，其区别官方文档说明为：

> QChart is a QGraphicsWidget that you can show in a QGraphicsScene. It manages the graphical representation of different types of series and other chart related objects like legend and axes. If you simply want to show a chart in a layout, you can use the convenience class QChartView instead of QChart.

>QChartView is a standalone widget that can display charts. It does not require separate QGraphicsScene to work. If you want to display a chart in your existing QGraphicsScene, you need to use the QChart (or QPolarChart) class instead.

因为这里是单独作为一个窗体放置，因此用QChartView 

![这里写图片描述](http://img.blog.csdn.net/20160514212852385)

提升的类名为QChartView 
头文件也为QChartView ，Qt每个类都有个无后缀的同类名文件，方便include
尝试编译一下程序会出现一个错误：

![这里写图片描述](http://img.blog.csdn.net/20160514213500767)

官方文档说明，使用QtChart还需要对pro文件进行设置：

>To create a simple application, start by creating a new Qt Gui Application project in Qt Creator and add this line to the .pro file of the project:
```C++
QT += charts
```

>In the main.cpp file, include the module headers and declare namespace usage:
```C++
#include <QtCharts>
using namespace QtCharts;
```

其中，可以使用`QT_CHARTS_USE_NAMESPACE`替代using namespace QtCharts;

![这里写图片描述](http://img.blog.csdn.net/20160514213757833)

添加完所需要的内容，编译还是发生错误，错误定位到ui_widget里

![这里写图片描述](http://img.blog.csdn.net/20160514213816367)

前面说了，用到QtChart的地方需要包含`#include <QtCharts>`以及命名空间`using namespace QtCharts;`（或者`QT_CHARTS_USE_NAMESPACE`替代using namespace QtCharts;）
但是ui_widget文件是自动生成的，不能再此文件里进行修改，此文件是包含在widget.h的一个叫Ui的命名空间中，因此，可以把
```C++
#include <QtCharts>
QT_CHARTS_USE_NAMESPACE
```
放置在widget.h上：
```C++
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
```

此时在编译即可：

![这里写图片描述](http://img.blog.csdn.net/20160514215647578)
在构造函数里添加：

```C++
QLineSeries* line1 = new QLineSeries();
for(double x=0;x<10;x+=0.1)
{
    line1->append(x,sin(x));
}
QChart* c = new QChart();
c->addSeries(line1);
ui->chart->setChart(c);
```

效果如下：

![这里写图片描述](http://img.blog.csdn.net/20160514221230100)

#总结
在使用界面设计师用QtChart时，需要在对应的界面头文件上添加：
```C++
#include <QtCharts>
QT_CHARTS_USE_NAMESPACE
```
否则会发生编译错误！

完！

#参考文献

[1][Qt Charts 5.7.0 安装教程http://www.jianshu.com/p/8d9a817aa6b9](http://www.jianshu.com/p/8d9a817aa6b9)

[2][Qt Chart 5.7.0 傻瓜安装教程http://www.cnblogs.com/findumars/p/5152294.html](http://www.cnblogs.com/findumars/p/5152294.html)

[3][QtCharts编译（编译环境MSVC12）和VS2013配置QtCharts http://blog.csdn.net/wokaowokaowokao12345/article/details/50933447](http://blog.csdn.net/wokaowokaowokao12345/article/details/50933447)