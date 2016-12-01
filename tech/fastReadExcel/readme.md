#Qt Windows 下快速读写Excel指南
很多人搜如何读写excel都会看到用`QAxObject`来进行操作，很多人试了之后都会发现一个问题，就是慢，非常缓慢！因此很多人得出结论是`QAxObject`读写excel方法不可取，效率低。
后来我曾试过用ODBC等数据库类型的接口进行读写，遇到中文嗝屁不说，超大的excel还是会读取速度慢。
最后，看了一些开源的代码后发现，Windows下读取excel，还是用`QAxObject`最快！没错，就是用`QAxObject`读写最快！！！
大家以后读取excel时（win下），不用考虑别的方法，用`QAxObject`就行，速度杠杠的，慢是你操作有误！下面就说说咋能提高其读取效率。

#读取excel慢的原因
这里不说如何打开或生成excel，着重说说如何快速读取excel。
网上搜到用Qt操作excel的方法，读取都是使用类似下面这种方法进行：

```C++
QVariant ExcelBase::read(int row, int col)
{
    QVariant ret;
    if (this->sheet != NULL && ! this->sheet->isNull())
    {
        QAxObject* range = this->sheet->querySubObject("Cells(int, int)", row, col);
        //ret = range->property("Value");
        ret = range->dynamicCall("Value()");
        delete range;
    }
    return ret;
}
```

读取慢的根源就在于`sheet->querySubObject("Cells(int, int)", row, col)`

试想有10000个单元就得调用10000次`querySubObject`，网络上90%的教程都没说这个`querySubObject`产生的`QAxObject*`最好进行手动删除，虽然在它的父级`QAxObject`会管理它的内存，但父级不析构，子对象也不会析构，若调用10000次，就会产生10000个`QAxObject`对象
得益于[QT快速读取数据量很大的Excel文件](http://blog.csdn.net/a1069962325/article/details/49514377)此文，下面总结如何快速读写excel

#快速读取excel文件

原则是一次调用`querySubObject`把所有数据读取到内存中
VBA中可以使用`UsedRange`把所有用到的单元格范围返回，并使用属性`Value`把这些单元格的所有值获取。

这时，获取到的值是一个table，但Qt把它变为一个变量QVariant来储存，其实实际是一个`QList<QList<QVariant> >`，此时要操作里面的内容，需要把这个`QVariant`转换为`QList<QList<QVariant> >`

先看看获取整个单元格的函数示意(这里ExcelBase是一个读写excel的类封装)：

```C++
QVariant ExcelBase::readAll()
{
    QVariant var;
    if (this->sheet != NULL && ! this->sheet->isNull())
    {
        QAxObject *usedRange = this->sheet->querySubObject("UsedRange");
        if(NULL == usedRange || usedRange->isNull())
        {
            return var;
        }
        var = usedRange->dynamicCall("Value");
        delete usedRange;
    }
    return var;
}
```

代码中`this->sheet`是已经打开的一个sheet，再获取内容时使用`this->sheet->querySubObject("UsedRange");`即可把所有范围都获取。

下面这个castVariant2ListListVariant函数把`QVariant`转换为`QList<QList<QVariant> >`
	
```C++
///
/// \brief 把QVariant转为QList<QList<QVariant> >
/// \param var
/// \param res
///
void ExcelBase::castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant> > &res)
{
    QVariantList varRows = var.toList();
    if(varRows.isEmpty())
    {
        return;
    }
    const int rowCount = varRows.size();
    QVariantList rowData;
    for(int i=0;i<rowCount;++i)
    {
        rowData = varRows[i].toList();
        res.push_back(rowData);
    }
}
```

这样excel的所有内容都转换为`QList<QList<QVariant> >`保存，其中`QList<QList<QVariant> >`中`QList<QVariant>`为每行的内容，行按顺序放入最外围的QList中。

对于如下如的excel

![](https://github.com/czyt1988/czyBlog/raw/master/tech/fastReadExcel/pic/02_0.png)

读取后的`QList<QList<QVariant> >`结构如下所示：

![](https://github.com/czyt1988/czyBlog/raw/master/tech/fastReadExcel/pic/02.png)

![](https://github.com/czyt1988/czyBlog/raw/master/tech/fastReadExcel/pic/03.png)

下面看看此excel的读取速度有多高
这里有个excel，有1000行，100列，共计十万单元格，打开使用了一些时间，读取10万单元格耗时229毫秒，
读取的代码如下：(完整源代码见后面)

```C++
void MainWindow::on_action_open_triggered()
{
    QString xlsFile = QFileDialog::getOpenFileName(this,QString(),QString(),"excel(*.xls *.xlsx)");
    if(xlsFile.isEmpty())
        return;
    QElapsedTimer timer;
    timer.start();
    if(m_xls.isNull())
        m_xls.reset(new ExcelBase);
    m_xls->open(xlsFile);
    qDebug()<<"open cost:"<<timer.elapsed()<<"ms";timer.restart();
    m_xls->setCurrentSheet(1);
    m_xls->readAll(m_datas);
    qDebug()<<"read data cost:"<<timer.elapsed()<<"ms";timer.restart();
    QVariantListListModel* md = qobject_cast<QVariantListListModel*>(ui->tableView->model());
    if(md)
    {
        md->updateData();
    }
    qDebug()<<"show data cost:"<<timer.elapsed()<<"ms";timer.restart();
}
```

上面的m_xls和m_datas是成员变量：

```C++
QScopedPointer<ExcelBase> m_xls;
QList< QList<QVariant> > m_datas;
```

读取的耗时：

	"D:\czy_blog\czyBlog\fastReadExcel\src\fastReadExcelInWindows\excelRWByCztr1988.xls"
	open cost: 1183 ms
	read data cost: 229 ms
	show data cost: 14 ms

10万个也就0.2秒而已

#快速写入excel文件

同理，能通过`QAxObject *usedRange = this->sheet->querySubObject("UsedRange");`实现快速读取，也可以实现快速写入

快速写入前需要些获取写入单元格的范围：`Range(const QString&)`
如excel的A1为第一行第一列，那么A1:B2就是从第一行第一列到第二行第二列的范围。

要写入这个范围，同样也是通过一个与之对应的`QList<QList<QVariant> >`，具体见下面代码：

```C++
///
/// \brief 写入一个表格内容
/// \param cells
/// \return 成功写入返回true
/// \see readAllSheet
///
bool ExcelBase::writeCurrentSheet(const QList<QList<QVariant> > &cells)
{
    if(cells.size() <= 0)
        return false;
    if(NULL == this->sheet || this->sheet->isNull())
        return false;
    int row = cells.size();
    int col = cells.at(0).size();
    QString rangStr;
    convertToColName(col,rangStr);
    rangStr += QString::number(row);
    rangStr = "A1:" + rangStr;
    qDebug()<<rangStr;
    QAxObject *range = this->sheet->querySubObject("Range(const QString&)",rangStr);
    if(NULL == range || range->isNull())
    {
        return false;
    }
    bool succ = false;
    QVariant var;
    castListListVariant2Variant(cells,var);
    succ = range->setProperty("Value", var);
    delete range;
    return succ;
}
```

此函数是把数据从A1开始写

函数中的`convertToColName`为把列数，转换为excel中用字母表示的列数，这个函数是用递归来实现的：

```C++
///
/// \brief 把列数转换为excel的字母列号
/// \param data 大于0的数
/// \return 字母列号，如1->A 26->Z 27 AA
///
void ExcelBase::convertToColName(int data, QString &res)
{
    Q_ASSERT(data>0 && data<65535);
    int tempData = data / 26;
    if(tempData > 0)
    {
        int mode = data % 26;
        convertToColName(mode,res);
        convertToColName(tempData,res);
    }
    else
    {
        res=(to26AlphabetString(data)+res);
    }
}
///
/// \brief 数字转换为26字母
///
/// 1->A 26->Z
/// \param data
/// \return
///
QString ExcelBase::to26AlphabetString(int data)
{
    QChar ch = data + 0x40;//A对应0x41
    return QString(ch);
}
```

看看写excel的耗时：

```C++
void MainWindow::on_action_write_triggered()
{
    QString xlsFile = QFileDialog::getExistingDirectory(this);
    if(xlsFile.isEmpty())
        return;
    xlsFile += "/excelRWByCztr1988.xls";
    QElapsedTimer timer;
    timer.start();
    if(m_xls.isNull())
        m_xls.reset(new ExcelBase);
    m_xls->create(xlsFile);
    qDebug()<<"create cost:"<<timer.elapsed()<<"ms";timer.restart();
    QList< QList<QVariant> > m_datas;
    for(int i=0;i<1000;++i)
    {
        QList<QVariant> rows;
        for(int j=0;j<100;++j)
        {
            rows.append(i*j);
        }
        m_datas.append(rows);
    }
    m_xls->setCurrentSheet(1);
    timer.restart();
    m_xls->writeCurrentSheet(m_datas);
    qDebug()<<"write cost:"<<timer.elapsed()<<"ms";timer.restart();
    m_xls->save();
}
```

输出：

	create cost: 814 ms 
	"A1:CV1000" 
	write cost: 262 ms 

写10万个数据耗时262ms，有木有感觉很快，很强大

#结论

- Qt在windows下读写excel最快速的方法还是使用QAxObject
- 不要使用类似`sheet->querySubObject("Cells(int, int)", row, col);`的方式读写excel，这是导致低效的更本原因


#源代码

[--> 见 github ](https://github.com/czyt1988/czyBlog/tree/master/tech/fastReadExcel)

![](https://github.com/czyt1988/czyBlog/raw/master/tech/fastReadExcel/pic/01.png)

