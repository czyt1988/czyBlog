#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#define SHOW_TRAP_1 1
#define SHOW_TRAP_2 1
typedef std::function<void()> FP;

class Point
{
public:
    Point(int x,int y):m_x(x),m_y(y)
    {

    }
    void print()
    {
#if 1
        int x = m_x;
        int y = m_y;
        s_print_history.push_back([x,y](){std::cout << "(X:" << x << ",Y:" << y <<")" << std::endl;});
#else
        s_print_history.push_back([=](){std::cout << "(X:" << m_x << ",Y:" << m_y <<")" << std::endl;});
#endif
        std::cout << "(X:" << m_x << ",Y:" << m_y <<")" << std::endl;
    }

    static void print_history()
    {
        std::cout << "print history:"<<std::endl;
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

void run_fun_ptr(FP fp);
FP get_fun_ptr();
FP get_fun_ptr_ref();


int main()
{
#if SHOW_TRAP_1
    run_fun_ptr(get_fun_ptr());
    run_fun_ptr(get_fun_ptr_ref());
#endif

#if SHOW_TRAP_2

    std::unique_ptr<Point> p;
    p.reset(new Point(1,1));p->print();
    p.reset(new Point(2,2));p->print();
    p.reset(new Point(3,3));p->print();
    Point::print_history();

#endif

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
    return [=](){std::cout << "= a:"<<a << std::endl;};
}
FP get_fun_ptr_ref()
{
    int a = 2;
    return [&](){std::cout << "& a:"<<a << std::endl;};
}






