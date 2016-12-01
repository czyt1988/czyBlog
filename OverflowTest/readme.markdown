#有符号无符号数据类型溢出测试记录

今天遇到一个比较基础问题：若有符号位的数据类型溢出会发生什么后果？
以前记得微机原理上说数据有原码、反码、补码3种表示形式，计算机通常的处理都是按照补码数据进行的，那么在一个数据类型的最大值溢出后会发生什么情况呢。

环境：Qt5.7
```cpp
#include <QCoreApplication>
#include <limits>
#include <iostream>
#include <QString>
#include <QDebug>
#include <sys/types.h>
#define TYPE_signed int
#define TYPE_NAME_signed "int"
#define TYPE_unsigned unsigned int
#define TYPE_NAME_unsigned "unsigned int"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    const int base = 10;
    QString baseStr = base == 10 ? "" : "0x";
    qDebug()<<QString(TYPE_NAME_signed" max:%1%2")
              .arg(baseStr)
              .arg(std::numeric_limits<TYPE_signed>::max(),0,base);
    qDebug()<<QString(TYPE_NAME_signed" min:%1%2")
              .arg(baseStr)
              .arg(std::numeric_limits<TYPE_signed>::min(),0,base);
    TYPE_signed symbTest;
    symbTest = std::numeric_limits<TYPE_signed>::max();
    symbTest += 1;
    qDebug()<<QString(TYPE_NAME_signed" Overflow result:%1%2+1=%1%3")
              .arg(baseStr)
              .arg(std::numeric_limits<TYPE_signed>::max(),0,base)
              .arg(symbTest,0,base);
    TYPE_signed tmp = symbTest + 1;
    qDebug()<<QString(TYPE_NAME_signed" Overflow result:%1%2+1=%1%3")
              .arg(baseStr)
              .arg(symbTest,0,base)
              .arg(tmp,0,base);


    qDebug()<<QString(TYPE_NAME_unsigned" max:%1%2")
              .arg(baseStr)
              .arg(std::numeric_limits<TYPE_unsigned>::max(),0,base);
    qDebug()<<QString(TYPE_NAME_unsigned" min:%1%2")
              .arg(baseStr)
              .arg(std::numeric_limits<TYPE_unsigned>::min(),0,base);
    TYPE_unsigned usymbText;
    usymbText = std::numeric_limits<TYPE_unsigned>::max();
    usymbText += 1;
    qDebug()<<QString(TYPE_NAME_unsigned" Overflow result:%1%2+1=%1%3")
              .arg(baseStr)
              .arg(std::numeric_limits<TYPE_unsigned>::max(),0,base)
              .arg(usymbText,0,base);
    return a.exec();
}
分别作了几种数据类型的测试：
```
int16_t和u_int16_t输出结果：

> "int16_t max:32767"
> "int16_t min:-32768"
> "int16_t Overflow result:32767+1=-32768"
> "int16_t Overflow result:-32768+1=-32767"
> "u_int16_t max:65535"
> "u_int16_t min:0"
> "int16_t Overflow result:65535+1=0"

int和unsigned int输出结果：
> "int max:2147483647"
> "int min:-2147483648"
> "int Overflow result:2147483647+1=-2147483648"
> "int Overflow result:-2147483648+1=-2147483647"
> "unsigned int max:4294967295"
> "unsigned int min:0"
> "unsigned int Overflow result:4294967295+1=0"

#结论
- 无符号位最大值加1后会变为0，重新重0开始加
- 有符号位最大值加1后会变为有符号位的最小值，继续在此基础上递增






