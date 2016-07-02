#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QDataStream>

void demo1(QFile& file);
void demo2(QFile& file);
const QString FILE_NAME = "czyQDataStreamDemo.file";
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile file(FILE_NAME);
    demo1(file);
    //demo2(file);
    return a.exec();
}

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
        out<<(data,256);
        file.close();
    }
    else
    {
        qDebug()<<file.errorString();
    }
}

void demo2(QFile& file)
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
}
