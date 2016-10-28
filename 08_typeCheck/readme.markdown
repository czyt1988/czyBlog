C++11的模板类型判断——std::is_same和std::decay

问题提出：有一个模板函数，函数在处理int型和double型时需要进行特殊的处理，那么怎么在编译期知道传入的参数的数据类型是int型还是double型呢？
如：
```cpp
#include <iostream>
template<typename TYPE>
void typeCheck(TYPE data)
{
    //do something check data type
    //std::cout<< out put the type
}
```
这里就需要用到C++11的`type_traits`头文件了，`type_traits`头文件定义了很多类型检查相关的方法，上面的例子具体用到了其中两个结构：

#std::is_same 判断类型是否一致

位于头文件`<type_traits>`中

这个结构体作用很简单，就是两个一样的类型会返回true
```cpp
bool isInt = std::is_same<int, int>::value; //为true
```
下面是官方的例子：
```cpp
#include <iostream>
#include <type_traits>
#include <cstdint>
 
void print_separator()
{
    std::cout << "-----\n";
}
 
int main()
{
    std::cout << std::boolalpha;
 
    std::cout << std::is_same<int, int32_t>::value << '\n';   // true
    std::cout << std::is_same<int, int64_t>::value << '\n';   // false
    std::cout << std::is_same<float, int32_t>::value << '\n'; // false
 
    print_separator();
 
    std::cout << std::is_same<int, int>::value << "\n";          // true
    std::cout << std::is_same<int, unsigned int>::value << "\n"; // false
    std::cout << std::is_same<int, signed int>::value << "\n";   // true
 
    print_separator();
 
    // unlike other types 'char' is not 'unsigned' and not 'signed'
    std::cout << std::is_same<char, char>::value << "\n";          // true
    std::cout << std::is_same<char, unsigned char>::value << "\n"; // false
    std::cout << std::is_same<char, signed char>::value << "\n";   // false
}
```
通过std::is_same即可判断两个类型是否一样，特别在模板里面，在不清楚模板的参数时，此功能可以对一些特定的参数类型进行特殊的处理。

> 这里说个题外话，大家是否通过std::is_same发现，char既不是unsigned char也不是signed char，char就是char，这和int是signed int的缩写是不一样的，char的表达范围可能等同于signed char，也可能等同于unsigned char，取决于编译器，一般是等同于signed char，但这个仅仅是范围等同，就像32位上int和long范围是一样的，但不是同一个类型。

> 因为用途不同，char用于表达字符，理论上不应该关心其正负的实现，而signed char 和 unsigned char 用于表达数值，或可移植的char。

回到正文，std::is_same可以判断两种类似是否一样，那么用在模板里就是利器了，本位一开始提到的那个问题就可以这样写：

```cpp
#include <iostream>
template<typename TYPE>
typeCheck(TYPE data)
{
    if(std::is_same<TYPE,int>::value)
    {
        std::cout<<"int type";
        //do something int 
    }
    else
    {
        //.........
    }
}
```

视乎很美好，再看一个示例：

```cpp
// is_same example
#include <iostream>
#include <type_traits>
#include <cstdint>

typedef int integer_type;
struct A { int x,y; };
struct B { int x,y; };
typedef A C;

int main() {
      std::cout << std::boolalpha;
      std::cout << "is_same:" << std::endl;
      std::cout << "int, const int: " << std::is_same<int, const int>::value << std::endl;//false
	  std::cout << "int, int&: " << std::is_same<int, int&>::value << std::endl;//false
	  std::cout << "int, const int&: " << std::is_same<int, const int&>::value << std::endl;//false
      std::cout << "int, integer_type: " << std::is_same<int, integer_type>::value << std::endl;//true
      std::cout << "A, B: " << std::is_same<A,B>::value << std::endl;//false
      std::cout << "A, C: " << std::is_same<A,C>::value << std::endl;//true
      std::cout << "signed char, std::int8_t: " << std::is_same<signed char,std::int8_t>::value << std::endl;//true
      return 0;
}
```
输出：
```
is_same:
int, const int: false
int, int&: false
int, const int&: false
int, integer_type: true
A, B: false
A, C: true
signed char, std::int8_t: true
```
发现`std::is_same`的判断是很严格的
但是有时候在编辑模板的时候又发现用`std::is_same`的判断太过严格，还是之前的例子：

```cpp
#include <stdlib.h>
#include <iostream>
#include <type_traits>


template<typename TYPE>
void typeCheck(TYPE data);

int _tmain(int argc, _TCHAR* argv[])
{
	int a = 1;
	const int& b = a;
	int& c = a;
	int d[12];
	const int& e = d[7];
	typeCheck(a);//int type
	typeCheck(b);//int type
	typeCheck(c);//int type
	typeCheck(d[7]);//int type
	typeCheck(e);//int type
	typeCheck(8);//int type
	system("pause");
	return 0;
}

template<typename TYPE>
void typeCheck(TYPE data)
{
    if(std::is_same<TYPE,int>::value)
    {
        std::cout<<"int type"<<std::endl;
    }
    else if(std::is_same<TYPE,std::string>::value)
    {
        std::cout<<"string type"<<std::endl;
    }
	else
	{
		std::cout<<"other type";
	}
}
```

输出：

```
int type
int type
int type
int type
int type
int type
```

测试后发现，虽然变量b,c使用引用，但`std::is_same`还是能识别出来的，**但是！！**
如果我显示的指定模板参数类型时情况有不一样了：

```cpp
#include <stdlib.h>
#include <iostream>
#include <type_traits>

template<typename TYPE>
void typeCheck(TYPE data);

int _tmain(int argc, _TCHAR* argv[])
{
	int a = 1;
	const int& b = a;
	int& c = a;
	int d[12];

	typeCheck<int>(a);        //int type
	typeCheck<const int&>(b);//other type
	typeCheck<int &>(c);        //other type
	typeCheck<const int&>(d[7]);//other type
	typeCheck(8);                //int type
	system("pause");
	return 0;
}

template<typename TYPE>
void typeCheck(TYPE data)
{
    if(std::is_same<TYPE,int>::value)
    {
        std::cout<<"int type"<<std::endl;
    }
    else if(std::is_same<TYPE,std::string>::value)
    {
        std::cout<<"string type"<<std::endl;
    }
	else
	{
		std::cout<<"other type";
	}
}
```
输出：
```
int type
other type
other type
other type
int type
```
瞬间结果就不一样了，这很好了解，从上面可知道，`std::is_same`对int和const int\int &\const int&等都是区别对待的，但在写模板函数时，经常会强制指定常引用进行传参，以免进行数据拷贝，这时候is_same就做出了不相等的判断，但是有时候其实我们还是希望TYPE和const TYPE& 是能认为是一样的，这时就需要`std::decay`进行退化处理

#std::decay 退化类型的修饰

`std::decay`就是对一个类型进行退化处理，他的实现如下:
```cpp
template< class T >
struct decay {
private:
    typedef typename std::remove_reference<T>::type U;
public:
    typedef typename std::conditional< 
        std::is_array<U>::value,
        typename std::remove_extent<U>::type*,
        typename std::conditional< 
            std::is_function<U>::value,
            typename std::add_pointer<U>::type,
            typename std::remove_cv<U>::type
        >::type
    >::type type;
};
```
看着比较抽象，其实就是把各种引用啊什么的修饰去掉，把cosnt int&退化为int，这样就能通过`std::is_same`正确识别出加了引用的类型了
上面的例子改为：
```cpp
#include "stdafx.h"
#include <stdlib.h>

#include <iostream>
#include <type_traits>


template<typename TYPE>
void typeCheck(TYPE data);

int _tmain(int argc, _TCHAR* argv[])
{
	int a = 1;
	const int& b = a;
	int& c = a;
	int d[12];

	typeCheck<int>(a);//int type
	typeCheck<const int&>(b);//int type
	typeCheck<int &>(c);//int type
	typeCheck<const int&>(d[7]);//int type
	typeCheck(8);//int type
	system("pause");
	return 0;
}

template<typename TYPE>
void typeCheck(TYPE data)
{
    if(std::is_same<typename std::decay<TYPE>::type,int>::value)
    {
        std::cout<<"int type"<<std::endl;
    }
	else
	{
		std::cout<<"other type"<<std::endl;
	}
}
```
在cppref有个更加详细的例子：
```cpp
#include <iostream>
#include <type_traits>
 
template <typename T, typename U>
struct decay_equiv : 
    std::is_same<typename std::decay<T>::type, U>::type 
{};
 
int main()
{
    std::cout << std::boolalpha
              << decay_equiv<int, int>::value << '\n'
              << decay_equiv<int&, int>::value << '\n'
              << decay_equiv<int&&, int>::value << '\n'
              << decay_equiv<const int&, int>::value << '\n'
              << decay_equiv<int[2], int*>::value << '\n'
              << decay_equiv<int(int), int(*)(int)>::value << '\n';
}
```
输出:
```
true
true
true
true
true
true
```
#总结：
- 在模板里可以通过std::is_same判断模板的类型，从而实现对不同类型的区别对待
- 在堆类型要求不是非常严格的情况下，可以使用std::decay把类型退化为基本形态，结合std::is_same用，可以判断出更多的情况

