# 概述
csv文件作为简单的格式化文本文件，随着数据挖掘和python的普及突然就又火起来了，工作中发现许多数据交互由原来的xml又改为通过csv文件进行交互，csv文件有个最大的缺点是单个单元格里不能出现换行，如果是单纯的数据交互，csv的确是最简单的格式化方式。
csv把每个单元数据用逗号隔开，但某些情况下需要注意的是，遇到一个单元内容有包含引号`"`和逗号`,`时是需要转义的。否则会造成格式混乱，本文的目的是封装一个简单好用的csv流式输入输出，类名`SACsvStream`，实现如下效果输出csv文件::

```cpp
QFile file;
……//open file
double d = 0.111;
float f = 0.343;
int i = 1212;
QString str = "这是一个文本";
SACsvStream csv(&file);
csv << d << f << i << str << endl;
```

实现输出：
```text
0.111,0.343,1212,这是一个文本
```

# 关键点

## csv的解析
其实导出csv和导出txt唯一的区别就是对csv的一些特殊情况需要转义操作，如遇到逗号遇到引号的情况
在csv中，遇到逗号需要把整个内容用引号扩起来
遇到引号，使用两个引号代表一个引号
如`这是一个"复合文本"文本:1,2,4,5,czyt1988`，在csv中需要显示为`"这是一个""复合文本""文本:1,2,4,5,czyt1988"`

##`operator <<`操作符对endl的处理
为了方便使用，重载了`operator <<`操作符，这里还需要注意一点，就是处理`endl`
在`operator <<`操作符中，`endl`其实是一个函数指针，为了能让`operator <<`操作符支持`endl`需要指定一个函数指针的`operator <<`操作符重载
例如:类SACsvStream 为了支持`operator <<`操作符识别endl，需要定义一个函数指针：
```cpp
typedef SACsvStream & (*SACsvWriterFunction)(SACsvStream &);
```
这个然后需要让`operator <<`操作符处理此函数指针:
```cpp
SACsvStream &operator<<(SACsvStream &s, SACsvWriterFunction f)
{
    return (*f)(s);
}
```
这样就可以写类似`SACsvStream &endl(SACsvStream &s);`这样的函数，实现需要的功能
```cpp
SACsvStream &endl(SACsvStream &s)
{
    //此处进行换行
    return s;
}
```
这样你不仅可以实现类似endl，你可以实现任何名称的重载
如：
```cpp
SACsvStream &wo(SACsvStream &s)
{
    s << "我";
    return s;
}
SACsvStream &zui(SACsvStream &s)
{
    s << "最";
    return s;
}
SACsvStream &shuai(SACsvStream &s)
{
    s << "帅";
    return s;
}
```
然后：
```cpp
SACsvStream  csv(&file);
csv << wo << zui << shuai;
```
输出结果大家自己默念

# 实现

头文件:
```cpp
#ifndef QCSVSTREAM_H
#define QCSVSTREAM_H
class QTextStream;
class QFile;
class SACsvStreamPrivate;
#include <QString>
///
/// \brief 写csv文件类支持
/// \author czy
/// \date 2016-08-10
///
class SACsvStream
{
public:
    SACsvStream(QTextStream* txt);
    SACsvStream(QFile* txt);
    virtual ~SACsvStream();
    //转换为标识csv字符
    static QString toCsvString(const QString& rawStr);
    //把一行要用逗号分隔的字符串转换为一行标准csv字符串
    static QString toCsvStringLine(const QStringList& sectionLine);
    //解析一行csv字符
    static QStringList fromCsvLine(const QString &lineStr);
    SACsvStream & operator << (const QString &str);
    SACsvStream & operator << (int d);
    SACsvStream & operator << (double d);
    SACsvStream & operator << (float d);
    //另起一行
    void newLine();
    //获取输入输出流
    QTextStream* stream() const;
    //读取并解析一行csv字符串
    QStringList readCsvLine();
    //判断是否到文件末端
    bool atEnd() const;
private:
    static int advquoted(const QString &s, QString &fld, int i);
    static int advplain(const QString &s, QString &fld, int i);
private:
    QScopedPointer<SACsvStreamPrivate> d_p;
};
typedef SACsvStream & (*SACsvWriterFunction)(SACsvStream &);
inline SACsvStream &operator<<(SACsvStream &s, SACsvWriterFunction f)
{
    return (*f)(s);
}
SACsvStream &endl(SACsvStream &s);
#endif // QCSVSTREAM_H

```

实现文件:
```cpp
#include "SACsvStream.h"
#include <QTextStream>
#include <QFile>
class SACsvStreamPrivate
{
    SACsvStream* Parent;
    QTextStream* m_txt;
    QScopedPointer<QTextStream> m_pfile;
    bool m_isStartLine;
public:
    SACsvStreamPrivate(SACsvStream* p):Parent(p)
      ,m_txt(nullptr)
      ,m_pfile(nullptr)
      ,m_isStartLine(true)
    {
    }
    void setTextStream(QTextStream* st)
    {
        m_txt = st;
    }
    QTextStream* stream()
    {
        return m_txt;
    }
    QTextStream& streamRef()
    {
        return (*m_txt);
    }
    bool isStartLine() const
    {
        return m_isStartLine;
    }
    void setStartLine(bool on)
    {
        m_isStartLine = on;
    }
    void setFile(QFile *txt)
    {
        m_pfile.reset(new QTextStream(txt));
        m_txt = m_pfile.data();
    }
    QTextStream& formatTextStream()
    {
        if(!isStartLine())
        {
            streamRef()<<',';
        }
        else
        {
            setStartLine(false);
        }
        return streamRef();
    }
};
SACsvStream::SACsvStream(QTextStream* txt)
    :d_p(new SACsvStreamPrivate(this))
{
    d_p->setTextStream(txt);
}
SACsvStream::SACsvStream(QFile *txt)
    :d_p(new SACsvStreamPrivate(this))
{
    d_p->setFile(txt);
}
SACsvStream::~SACsvStream()
{
}
///
/// \brief 把字符串装换为标准csv一个单元得字符串，对应字符串原有的逗号会进行装换
///
/// csv的原则是：
///
/// - 如果字符串有逗号，把整个字符串前后用引号括起来
/// - 如果字符串有引号",引号要用两个引号表示转义""
/// \param rawStr 原有数据
/// \return 标准的csv单元字符串
///
QString SACsvStream::toCsvString(const QString &rawStr)
{
    //首先查找有没有引号,如果有，则把引号替换为两个引号
    QString res = rawStr;
    res.replace("\"","\"\"");
    if(res.contains(','))
    {
        //如果有逗号，在前后把整个句子用引号包含
        res = ('\"' + res + '\"');
    }
    return res;
}
///
/// \brief 把一行要用逗号分隔的字符串转换为一行标准csv字符串
/// \param sectionLine 如：xxx,xxxx,xxxxx 传入{'xxx','xxxx','xxxxx'}
/// \return 标准的csv字符串不带换行符
///
QString SACsvStream::toCsvStringLine(const QStringList &sectionLine)
{
    QString res;
    int size = sectionLine.size();
    for(int i=0;i<size;++i)
    {
        if(0 == i)
        {
            res += SACsvStream::toCsvString(sectionLine[i]);
        }
        else
        {
            res += ("," + SACsvStream::toCsvString(sectionLine[i]));
        }
    }
    return res;
}
///
/// \brief 把一句csv格式的内容解析
/// \param lineStr
/// \return
///
QStringList SACsvStream::fromCsvLine(const QString &lineStr)
{
    if(lineStr.isEmpty())
    {
        return QStringList();
    }
    QStringList res;
    QString str;
    int i, j=0;
    int col = 0;
    i = 0;
    do {
        if (i < lineStr.length() && lineStr[i] == '\"')
            j = advquoted(lineStr, str, ++i); // skip quote
        else
            j = advplain(lineStr, str, i);
        res.push_back (str);
        ++col;
        i = j + 1;
    } while (j < lineStr.length());
    return res;
}
///
/// \brief 写csv文件内容，字符会自动转义为csv文件支持的字符串，不需要转义
///
/// 例如csv文件：
/// \par
/// 12,txt,23,34
/// 45,num,56,56
/// 写入的方法为：
/// \code
/// .....
/// QCsvWriter csv(&textStream);
/// csv<<"12"<<"txt"<<"23";
/// csv.endLine("34");
/// csv<<"45"<<"num"<<"56";
/// csv.endLine("56");
/// \endcode
///
/// \param str 需要写入的csv文件一个单元得字符串
/// \return
///
SACsvStream &SACsvStream::operator <<(const QString &str)
{
    d_p->formatTextStream()<<toCsvString(str);
    return *this;
}
SACsvStream &SACsvStream::operator <<(int d)
{
    d_p->formatTextStream()<<d;
    return *this;
}
SACsvStream &SACsvStream::operator <<(double d)
{
    d_p->formatTextStream()<<d;
    return *this;
}
SACsvStream &SACsvStream::operator <<(float d)
{
    d_p->formatTextStream()<<d;
    return *this;
}
///
/// \brief 换行
///
void SACsvStream::newLine()
{
    d_p->setStartLine(true);
    d_p->streamRef()<<endl;
}
///
/// \brief 获取输入输出流
/// \return
///
QTextStream *SACsvStream::stream() const
{
    return d_p->stream();
}
///
/// \brief 读取一行csv文件
/// \return
///
QStringList SACsvStream::readCsvLine()
{
    return fromCsvLine(stream()->readLine());
}
bool SACsvStream::atEnd() const
{
    return stream()->atEnd();
}
int SACsvStream::advquoted(const QString &s, QString &fld, int i)
{
    int j;
    fld = "";
    for (j = i; j < s.length()-1; j++)
    {
        if (s[j] == '\"' && s[++j] != '\"')
        {
            int k = s.indexOf (',', j);
            if (k < 0 ) // 没找到逗号
                k = s.length();
            for (k -= j; k-- > 0; )
                fld += s[j++];
            break;
        }
        fld += s[j];
    }
    return j;
}
int SACsvStream::advplain(const QString &s, QString &fld, int i)
{
    int j;
    j = s.indexOf(',', i); // 查找,
    if (j < 0) // 没找到
        j = s.length();
    fld = s.mid (i,j-i);//提取内容
    return j;
}
///
/// \brief endl
/// \param s
/// \return
///
SACsvStream &endl(SACsvStream &s)
{
    s.newLine();
    return s;
}


```

# 示例

写入测试:
```cpp
    QFile file("SACsvStreamTest.csv");
    if(!file.open(QIODevice::WriteOnly))
    {
        return;
    }
    double d = 0.111;
    float f = 0.343;
    int i = 1212;
    QString str1 = QStringLiteral("这是一个文本");
    QString str2 = QStringLiteral("这是一个带,号文本");
    QString str3 = QStringLiteral("这是一个带\"文本");
    QString str4 = QStringLiteral("这是一个\"复合文本\"文本:1,2,4,5,czyt1988");
    SACsvStream csv(&file);
    csv << d << f << i << str1 << str2 << str3 << str4 << endl;
    csv << str4 << str3 << str2 << str1 << i << f << d << endl;
    file.close();
```
结果：

```text
0.111,0.343,1212,这是一个文本,"这是一个带,号文本",这是一个带""文本,"这是一个""复合文本""文本:1,2,4,5,czyt1988"
"这是一个""复合文本""文本:1,2,4,5,czyt1988",这是一个带""文本,"这是一个带,号文本",这是一个文本,1212,0.343,0.111
```
读取示例：
```cpp
    QFile file("SACsvStreamTest.csv");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return -1;
    }
    SACsvStream csv(&file);
    while(!csv.atEnd())
    {
        QStringList res = csv.readCsvLine();
        qDebug().noquote() << res;
    }
```
输出:
```text
(0.111, 0.343, 1212, 这是一个文本, 这是一个带,号文本, 这是一个带""文本, 这是一个"复合文本"文本:1,2,4,5,czyt1988, )
(这是一个"复合文本"文本:1,2,4,5,czyt1988, 这是一个带""文本, 这是一个带,号文本, 这是一个文本, 1212, 0.343, 0.111)
```

# 支持
此文主要是对sa的SACsvStream 类进行介绍，SACsvStream 是sa用于处理csv的关键类，也是简单日志系统的一个支持
源码见[github](https://github.com/czyt1988/sa)：
 [https://github.com/czyt1988/sa/blob/master/src/signALib/SACsvStream.h](https://github.com/czyt1988/sa/blob/master/src/signALib/SACsvStream.h)
[https://github.com/czyt1988/sa/blob/master/src/signALib/SACsvStream.cpp](https://github.com/czyt1988/sa/blob/master/src/signALib/SACsvStream.cpp)
