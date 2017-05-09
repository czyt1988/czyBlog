#QDataStream  writeBytes与writeRawData区别

有次序列化文件时，要求需要写一个固定大小的char[256]，无论有没有遇到`\0`都要写入char[256]内容，Qt下使用`QDataStream`是非常容易操作文件读写的，但，在写这个固定长度的char数组时还是遇到了一些小问题。

下面就讨论怎么样很好的把固定的char[256]这片区域写到文件中：

`QDataStream`有流式操作`<<`，对文件操作非常方便

首先看看最常见的流输出：

    void demo1(QFile& file)
    {
        qDebug()<<__FUNCTION__;
        if(file.open(QIODevice::WriteOnly))
        {
            QDataStream out(&file);
            char data[256];
            memset(data,0,256*sizeof(char));
            char* p = "this is QDataStream demo";
            strcpy(data,p);
            out<<data;
            file.close();
        }
        else
        {
            qDebug()<<file.errorString();
        }
    }

上诉代码生成的是这样的：

    0000 0019 7468 6973 2069 7320 5144 6174 6153 7472 6561 6d20 6465 6d6f 00

显示的效果：

![](https://github.com/czyt1988/czyBlog/raw/master/03_QDataStream/pic/01.png)


这样很明显不会得到想要的效果，因为`QDataStream`遇到`\0`就自动结束了。而且前面还加了一些其他的内容。会有 `00 00 00 19` （t的ASCII为74）

要写入完整的char数组查手册，发现有`QDataStream::writeBytes`函数，于是试试writeBytes:

    void demo2QDataStream(QFile& file)
    {
        qDebug()<<__FUNCTION__;
        if(file.open(QIODevice::WriteOnly))
        {
            QDataStream out(&file);
            char data[256];
            memset(data,0,256*sizeof(char));
            char* p = "this is QDataStream demo";
            strcpy(data,p);
            out.writeBytes(data,256);
            file.close();
        }
        else
        {
            qDebug()<<file.errorString();
        }

        if(file.open(QIODevice::ReadOnly))
        {
            QDataStream out(&file);
            char *data;
            uint len;
            out.readBytes(data,len);
            if(len > 0)
            {
                qDebug()<<data;
                delete[] data;
            }
            file.close();
        }
        else
        {
            qDebug()<<file.errorString();
        }
    }


此时生成的文件如下：
![](https://github.com/czyt1988/czyBlog/raw/master/03_QDataStream/pic/02.png)
这时候，发现能完整的把char[256]写入到文件中，但前面的`00 00 01 00`是什么东西？

注意，使用`writeBytes`函数写的文件，必须用`readBytes`函数读取，`readBytes`函数是个工厂函数，会调用`new[]`分配内存，因此需要调用`delete[]` 进行删除内存，否则会泄露。

之前说的`00 00 01 00`是Qt特有的标示，用于Qt自身的验证，但要写一个通用点的文件，可以让别的环境也能很轻松的读取，需要用到另外一个函数`int QDataStream::writeRawData(const char * s, int len)`


`int QDataStream::writeRawData(const char * s, int len)`和`int QDataStream::readRawData(char * s, int len)`函数配对使用，用于写入最原始的内存，如果不想要Qt独有的一些信息，就需要用这两个函数了：

    void demo3QDataStream(QFile& file)
    {
        qDebug()<<__FUNCTION__;
        if(file.open(QIODevice::WriteOnly))
        {
            QDataStream out(&file);
            char data[256];
            memset(data,0,256*sizeof(char));
            char* p = "this is QDataStream demo";
            strcpy(data,p);
            out.writeRawData(data,256);
            file.close();
        }
        else
        {
            qDebug()<<file.errorString();
        }

        if(file.open(QIODevice::ReadOnly))
        {
            QDataStream out(&file);
            char data[256];
            memset(data,1,256*sizeof(char));
            out.readRawData(data,256);
            qDebug()<<data;
            file.close();
        }
        else
        {
            qDebug()<<file.errorString();
        }
    }

上面代码输出：

![](https://github.com/czyt1988/czyBlog/raw/master/03_QDataStream/pic/03.png)

生成的文件为：

![](https://github.com/czyt1988/czyBlog/raw/master/03_QDataStream/pic/04.png)

就是我们想要的不加各种乱七八糟的头尾的原始二进制了

#总结

在写出二进制时，如果需要仅仅写入原始的内存，记得使用`int QDataStream::writeRawData(const char * s, int len)`函数，而不是`QDataStream::writeBytes`函数！
