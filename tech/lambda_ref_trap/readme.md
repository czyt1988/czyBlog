# lambada表达式的陷阱

lambda表达式给stl带来了无与伦比的便利，尤其对像`std::for_each`这种使用函数指针的场合更是方便，但却是写的爽快，维护的蛋疼，前几天还遇到了一个陷阱，这里特意记录一下

# 陷阱1：默认引用捕获可能带来的悬挂引用问题

在捕获参数时喜欢使用`[&]`来一次捕获包括`this`在内的所有内容,此方法非常方便，但在遇到局部变量时，引用捕获却是非常容易出现问题，尤其用在事件系统,信号槽系统里时。

一个简单的lambda程序如下：

```cpp
#include <iostream>
#include <functional>
using namespace std;

typedef std::function<void()> FP;
void run_fun_ptr(FP fp);
FP get_fun_ptr();
FP get_fun_ptr_ref();
int main()
{
    run_fun_ptr(get_fun_ptr());
    run_fun_ptr(get_fun_ptr_ref());
    return 0;
}

void run_fun_ptr(FP fp)
{
    if(fp)
    {
        fp();
    }
}

FP get_fun_ptr()
{
    int a = 2;
    return [=](){cout << "= a:"<<a << endl;};
}

FP get_fun_ptr_ref()
{
    int a = 2;
    return [&](){cout << "& a:"<<a << endl;};
}


```

结果输出：

```text
= a:2
& a:4200153
```

这里`get_fun_ptr`正常输出，因为使用的是`=`号捕获,但`get_fun_ptr_ref`使用的是引用捕获，就会出现未定义的行为，因为捕获了一个临时变量，引用实际可以看成指针，在`get_fun_ptr_ref`之后，`get_fun_ptr_ref`中`int a = 2;`的临时变量会释放(出栈)，此时指针就不知道指的是什么东西了。

在有事件循环系统时，最典型的就是`ui`程序，若lambda的触发是依据某个事件，如一个鼠标对按钮的点击，但lambda却引用捕获了一个局部变量，在创建时变量存在，但在触发点击时，变量很有可能已经销毁了，这时就会有未定义错误发生。

如下例子是[SA](https://github.com/czyt1988/sa)的一个生成最近打开文件菜单项目的例子,作用就是把记录最近打开的文件路径生成一系列菜单项目，在第二个lambda表达式中，若用默认引用捕获，会把`QAction* act`作为引用捕获，在此函数结束后，将变成悬空引用

```cpp
    std::for_each(m_recentOpenFiles.begin(),m_recentOpenFiles.end(),[&](const QString& strPath){
        QAction* act = new QAction(strPath,this);
        connect(act,&QAction::triggered,this,[this,act](bool on){
            Q_UNUSED(on);
            this->openFile(act->text());
        });
        ui->menuRecentOpenFile->addAction(act);
    });
```

# 陷阱2：捕获this陷阱

后来在网上看到这篇文章[Effective Modern C++ 条款31 对于lambda表达式，避免使用默认捕获模式](https://blog.csdn.net/big_yellow_duck/article/details/52468051)

看来这是Modern C++的新坑，还好Effective系列把这些都点明了，文章除了上面说的捕获悬挂引用情况，还有一种情况会导致问题，就是lambda使用当前类外的变量时要异常小心其捕获的`this`指针,如lambda使用了全局变量，或者lambda所在类以外生命周期比这个类长的变量

```cpp
#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
typedef std::function<void()> FP;

class Point
{
public:
    Point(int x,int y):m_x(x),m_y(y)
    {

    }
    void print()
    {
        s_print_history.push_back([=](){std::cout << "(X:" << m_x << ",Y:" << m_y <<")" << std::endl;});
        std::cout << "(X:" << m_x << ",Y:" << m_y <<")" << std::endl;
    }

    static void print_history()
    {
        std::for_each(s_print_history.begin(),s_print_history.end(),[](FP p){
            if(p)
                p();
        });
    }

private:
    int m_x;
    int m_y;
    typedef std::function<void(void)> FP;
    static std::vector<FP> s_print_history;
};
std::vector<FP> Point::s_print_history = std::vector<FP>();

int main()
{
    std::unique_ptr<Point> p;
    p.reset(new Point(1,1));p->print();
    p.reset(new Point(2,2));p->print();
    p.reset(new Point(3,3));p->print();
    Point::print_history();
    return 0;
}
```

输出结果
```Text
(X:1,Y:1)
(X:2,Y:2)
(X:3,Y:3)
print history:
(X:3,Y:3)
(X:2,Y:2)
(X:3,Y:3)
```

这个历史输出明显不是正确的结果，这个历史已经是一个未定义的行为，别的编译器输出的结果和我这里编译的结果应该是不一样的，这里就是this的捕获陷阱

```cpp
s_print_history.push_back([=](){std::cout << "(X:" << m_x << ",Y:" << m_y <<")" << std::endl;});
```

这句lambda通过默认值捕获，其实只是捕获了this指针，在lambda里使用`m_x`,相当于`this->m_x`。在this销毁后在调用这个lambda，这时候的this就不知指到哪里了。

由于lambda里有比创建这个lambda的this生命周期更长的变量，一般是引入这个类的其他类型变量或者是静态变量和全局变量，一旦涉及到这三种东西，不建议用lambda，但任性要用，需要做一个中转，上述打印代码应该改为：

```cpp
void print()
{
    int x = m_x;
    int y = m_y;
    s_print_history.push_back([x,y](){std::cout << "(X:" << x << ",Y:" << y <<")" << std::endl;});
    std::cout << "(X:" << m_x << ",Y:" << m_y <<")" << std::endl;
}
```

这时，会把x，y通过传值捕获，lambda里没有保存`this`指针信息，避免隐藏`this`的影响。

具体建议大家看看这篇文章[Effective Modern C++ 条款31 对于lambda表达式，避免使用默认捕获模式](https://blog.csdn.net/big_yellow_duck/article/details/52468051)

# 总结

- 引用捕获陷阱：引用捕获`[&]`别使用局部变量

- this陷阱：`lambda`里避免有全局变量或静态变量或者比当前类生命周期更长的变量

- 尽量避免使用复杂的`lambda`