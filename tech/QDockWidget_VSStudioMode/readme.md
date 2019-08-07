# 概述

许多工程软件，如Qt Creator，VS，matlab等，都是使用dock布局窗口，这样用户可以自定义界面，自由组合窗口。
Qt的嵌套布局由QDockWidget完成，用Qt Creator拖界面得到的dock布置形式比较固定，不能得想要的任意组合形式，要得到如下图所示的效果，后续布局必须通过代码来完成。 

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QDockWidget_VSStudioMode/pic/1.gif)

ps:这是自己没事写的一个数据可视化软件
下面说说如何实现完全自由的界面布局效果：

# QDockWidget在QMainWindow的布局函数

要在QMainWindow里对dock进行布局，需要用到如下几个函数：

- 添加dock函数 

此函数用于给dock指定位置，同时也可以更改dock的位置，此函数命名为addDockWidget有点容易误导，因为不仅仅有add的功能，还有chang的功能

```cpp
void QMainWindow::addDockWidget(Qt::DockWidgetArea area, QDockWidget * dockwidget)
void QMainWindow::addDockWidget(Qt::DockWidgetArea area, QDockWidget * dockwidget, Qt::Orientation orientation)
```

- 分割dock窗口函数 

此函数的功能是把两个dock进行左右或上下并排布置，做成一个类似QSplit的功能

```cpp
void QMainWindow::splitDockWidget(QDockWidget * first, QDockWidget * second, Qt::Orientation orientation)
```

- tab化窗口函数

此函数的功能是把多个dock变成一个tab形式的窗体

```cpp
void QMainWindow::tabifyDockWidget(QDockWidget * first, QDockWidget * second)
```

- 设置dock嵌套布局

此函数是设置嵌套布局的关键

```cpp
void QMainWindow::setDockNestingEnabled(bool enabled)
```

以上几个函数就能完成比较复杂的嵌套布局了。

# 设置嵌套布局

下面通过例子来讲解如何设置复杂的嵌套布局
先用Qt Creator拖放9个dock进视图里，为了好区分，给每个dock设置一个背景颜色： 

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QDockWidget_VSStudioMode/pic/2.png)

dock属性随便设置，保证都任意区域可以停靠即可
由于这里不需要MainWindow的中间窗口，整个视图都由dock组成，因此先把QMainWindow的中间窗口部件去除：
在MainWindow的构造函数加入如下语句，即可把MainWindow的中间窗口去除，这时整个MainWindow只有Dock组成

```cpp
QWidget* p = takeCentralWidget();
    if(p)
        delete p;
```

编译出来的效果如图所示： 

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QDockWidget_VSStudioMode/pic/3.gif)

拖动dock可以发现，只能在两边进行组合，我想把dock放置到中间是无法实现的，这是由于为了简化dock的吸附，QMainWindow默认是把dock嵌套关闭的，需要我们手动设置，在MainWindow的构造函数里添加：

```cpp
setDockNestingEnabled(true);
```

即可打开嵌套功能，这时编译出来的窗口能实现如下嵌套：

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QDockWidget_VSStudioMode/pic/4.gif)

此时，整个窗口的布局将变得非常灵活且复杂，由于Qt Creator在ui编辑器中无法像编译出来的程序那样任意调整位置，因此需要手动对窗口进行设置。下面将介绍如何用代码设置复杂的dock
为了方便，添加两个函数和一个成员变量：
head：

```cpp
public:
    //移除并隐藏所有dock
    void removeAllDock();
    //显示dock窗口
    void showDock(const QList<int>& index = QList<int>());
private:
    QList<QDockWidget*> m_docks;///< 记录所有dockWidget的指针
```

CPP:

```cpp
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
```

`void removeAllDock();`函数可以把所有的dock隐藏`void showDock(const QList<int>& index = QList<int>())`则可以显示指定的dock。
下面先对需要用到的几个函数进行示范：

## addDockWidget

`addDockWidget`函数用于给`MainWindow`添加dock窗体，指定添加的区域，如果想改变dock的位置，也可以使用此函数进行移动。

```cpp
void QMainWindow::addDockWidget(Qt::DockWidgetArea area, QDockWidget * dockwidget)
void QMainWindow::addDockWidget(Qt::DockWidgetArea area, QDockWidget * dockwidget, Qt::Orientation orientation)
```

如：

```cpp
addDockWidget(Qt::LeftDockWidgetArea,ui->dockWidget_1);
addDockWidget(Qt::RightDockWidgetArea,ui->dockWidget_2);
addDockWidget(Qt::TopDockWidgetArea,ui->dockWidget_3);
addDockWidget(Qt::BottomDockWidgetArea,ui->dockWidget_4);
```

把4个dock按照上下左右布置，效果如下：

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QDockWidget_VSStudioMode/pic/5.png)

## splitDockWidget

```cpp
void QMainWindow::splitDockWidget(QDockWidget * first, QDockWidget * second, Qt::Orientation orientation)
```

此函数可以把一个`dock（QDockWidget * first）`在其位置上进行嵌套，嵌套可以指定水平嵌套或者垂直嵌套，嵌套方向是从左到右，从上到下，也就是`QDockWidget * first`相对于`QDockWidget * second`永远在左边或者上边。
如：

```cpp
removeAllDock();
addDockWidget(Qt::LeftDockWidgetArea,ui->dockWidget_1);
splitDockWidget(ui->dockWidget_1,ui->dockWidget_2,Qt::Horizontal);
splitDockWidget(ui->dockWidget_2,ui->dockWidget_3,Qt::Horizontal);
splitDockWidget(ui->dockWidget_3,ui->dockWidget_4,Qt::Horizontal);
splitDockWidget(ui->dockWidget_4,ui->dockWidget_5,Qt::Horizontal);
showDock(QList<int>()<< 0<<1<<2<<3<<4);
```

得到如下效果：

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QDockWidget_VSStudioMode/pic/6.png)

若是：

```cpp
removeAllDock();
addDockWidget(Qt::LeftDockWidgetArea,ui->dockWidget_1);
splitDockWidget(ui->dockWidget_1,ui->dockWidget_2,Qt::Vertical);
splitDockWidget(ui->dockWidget_2,ui->dockWidget_3,Qt::Vertical);
splitDockWidget(ui->dockWidget_3,ui->dockWidget_4,Qt::Vertical);
splitDockWidget(ui->dockWidget_4,ui->dockWidget_5,Qt::Vertical);
showDock(QList<int>()<< 0<<1<<2<<3<<4);
```

那么效果变为：

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QDockWidget_VSStudioMode/pic/7.png)

此函数是实现嵌套布局的关键，首先指定基准，然后开始进行分割，即可得到比较复杂的布局。
分割原则是：**先水平，再竖直，从左到右，从上到下**

下面显示一个九宫格布局： 

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QDockWidget_VSStudioMode/pic/8.png)

实现代码

```cpp
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
```

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QDockWidget_VSStudioMode/pic/9.png)

```cpp
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
```

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QDockWidget_VSStudioMode/pic/10.png)

实现代码：

```cpp
removeAllDock();
addDockWidget(Qt::TopDockWidgetArea,ui->dockWidget_1);
addDockWidget(Qt::LeftDockWidgetArea,ui->dockWidget_2);
splitDockWidget(ui->dockWidget_2,ui->dockWidget_3,Qt::Horizontal);
splitDockWidget(ui->dockWidget_3,ui->dockWidget_4,Qt::Horizontal);
splitDockWidget(ui->dockWidget_2,ui->dockWidget_5,Qt::Horizontal);
splitDockWidget(ui->dockWidget_3,ui->dockWidget_6,Qt::Horizontal);
splitDockWidget(ui->dockWidget_4,ui->dockWidget_7,Qt::Horizontal);
showDock(QList<int>()<< 0<<1<<2<<3<<4<<5<<6);
```

反正就是用`splitDockWidget`和`addDockWidget`你想怎么布置就怎么布置。

## tabifyDockWidget

此函数就是实现tab合并功能
直接看看下面例子： 

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QDockWidget_VSStudioMode/pic/11.png)

实现原理：

```cpp
removeAllDock();
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
```
