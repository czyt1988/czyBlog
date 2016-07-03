#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QDataStream>

void demo1QDataStream(QFile& file);
void demo2QDataStream(QFile& file);
void demo3QDataStream(QFile& file);
void demo4QDataStream(QFile& file);
void demo2_1QDataStream(QFile& file);
const QString FILE_NAME = "czyQDataStreamDemo.file";
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile file(FILE_NAME);
    //demo1QDataStream(file);
    //demo2QDataStream(file);
    //demo3QDataStream(file);
    demo4QDataStream(file);
    //demo2_1QDataStream(file);
    return a.exec();
}

void demo1QDataStream(QFile& file)
{
    qDebug()<<__FUNCTION__;
    if(file.open(QIODevice::WriteOnly))
    {
        file.seek(0);
        QDataStream out(&file);
        char data[256];
        memset(data,0,256*sizeof(char));
        const char* p = "this is QDataStream demo";
        strcpy(data,p);
        out<<(data,256);
        file.close();
    }
    else
    {
        qDebug()<<file.errorString();
    }
}

void demo2QDataStream(QFile& file)
{
    qDebug()<<__FUNCTION__;
    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        char data[256];
        memset(data,0,256*sizeof(char));
        const char* p = "this is QDataStream demo";
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

void demo3QDataStream(QFile& file)
{
    qDebug()<<__FUNCTION__;
    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        char data[256];
        memset(data,0,256*sizeof(char));
        const char* p = "this is QDataStream demo";
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

void demo4QDataStream(QFile& file)
{
    qDebug()<<__FUNCTION__;
    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        char data[256];
        memset(data,0,256*sizeof(char));
        const wchar_t * p = L"this is QDataStream demo 这里还有中文！";
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

void demo2_1QDataStream(QFile& file)
{
    qDebug()<<__FUNCTION__;
    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        int i = 0;
        for (;i<10;++i)
        {
            out<<i;
        }
        file.close();
    }
    else
    {
        qDebug()<<file.errorString();
    }

    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream out(&file);
        int i = 0;
        for (;i<10;++i)
        {
            int data;
            out>>data;
            qDebug() << data;
        }
        file.close();
    }
    else
    {
        qDebug()<<file.errorString();
    }
}
