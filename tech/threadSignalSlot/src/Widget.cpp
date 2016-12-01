#include "Widget.h"
#include "ui_Widget.h"


TestThread::TestThread(QObject *par):QThread(par)
  ,m_abort(false)
{

}

TestThread::~TestThread()
{
    m_abort = true;
    wait();
}

void TestThread::run()
{
    while(!m_abort)
    {
        int a = 1;
        unsigned int add = (unsigned int)&a;
        emit signalData(a,add);
        a = 123213;
        sleep(1);
        m_abort = true;
    }
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_thread = new TestThread(this);
    connect(this,SIGNAL(signalData(int,unsigned int))
            ,SLOT(recDataQueuedConnection(int,unsigned int))
            ,Qt::QueuedConnection
            );
    connect(this,SIGNAL(signalData(int,unsigned int))
            ,SLOT(recDataQueuedConnection(int,unsigned int))
            ,Qt::DirectConnection
            );
    connect(m_thread,SIGNAL(signalData(int,unsigned int))
            ,SLOT(recThreadData(int,unsigned int))
            );

    QByteArray bt;
//    bt.append((char)0x55).append((char)0x55)
//            .append((char)0x3).append((char)0x0)
//            .append((char)0x3).append((char)0x4).append((char)0x0)
//            .append((char)0xa1).append((char)0x4a)
//            .append((char)0xaa).append((char)0xaa);
//    readData(bt);
    //
//    ui->textBrowser->append("unexpect text");
//    bt.clear();
//    bt.append((char)0x3).append((char)0x4).append((char)0x0)
//            .append((char)0x55).append((char)0x55)
//            .append((char)0x3).append((char)0x0)
//            .append((char)0x3);
//    readData(bt);
//    bt.clear();
//    bt
//            .append((char)0x4).append((char)0x0)
//            .append((char)0xa1).append((char)0x4a)
//            .append((char)0xaa).append((char)0xaa);
//    readData(bt);

//    ui->textBrowser->append("unexpect text 2");
//    bt.clear();
//    bt.append((char)0x3).append((char)0x4).append((char)0x0).append((char)0x0).append((char)0x0)
//            .append((char)0x55).append((char)0x55)
//            .append((char)0x3).append((char)0x0)
//            ;
//    readData(bt);
//    bt.clear();
//    bt.
//            append((char)0x3).append((char)0x4).append((char)0x0)
//            .append((char)0xa1).append((char)0x4a)
//            .append((char)0xaa).append((char)0xaa);
//    readData(bt);

//    ui->textBrowser->append("unexpect text 3");
//    bt.clear();
//    bt.append((char)0x3).append((char)0x4).append((char)0x0).append((char)0x0).append((char)0x0)
//            .append((char)0x55).append((char)0x55)
//            .append((char)0x3)
//            ;
//    readData(bt);
//    bt.clear();
//    bt.
//            append((char)0x0).append((char)0x3).append((char)0x4).append((char)0x0)
//            .append((char)0xa1).append((char)0x4a)
//            .append((char)0xaa).append((char)0xaa);
//    readData(bt);


//    ui->textBrowser->append("unexpect text 4");
//    bt.clear();
//    bt.append((char)0x3).append((char)0x4).append((char)0x0).append((char)0x0).append((char)0x0)
//            .append((char)0x55).append((char)0x55)
//            .append((char)0x5)
//            ;
//    readData(bt);
//    bt.clear();
//    bt.
//            append((char)0x0)
//            .append((char)0x3).append((char)0x4).append((char)0x0)
//            .append((char)0x55).append((char)0x55)
//            .append((char)0xa1).append((char)0x4a)
//            ;
//    readData(bt);
//    bt.clear();
//    bt.
//            append((char)0xaa).append((char)0xaa)
//            ;
//    readData(bt);

    ui->textBrowser->append("unexpect text 4");
    bt.clear();
    bt.append((char)0x3).append((char)0x4).append((char)0x0).append((char)0x0).append((char)0x0)
            .append((char)0x55).append((char)0x55)
            .append((char)0x5)
            ;
    readData(bt);
    bt.clear();
    bt.
            append((char)0x0)
            .append((char)0x3).append((char)0x4).append((char)0x0)
            .append((char)0x55).append((char)0x55)
            .append((char)0xa1).append((char)0x4a)
            ;
    readData(bt);
    bt.clear();
    bt.
            append((char)0xaa).append((char)0xaa)
            ;
    readData(bt);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_test_clicked()
{
    if(!m_thread->isRunning())
    {
        m_thread->start();
    }
    int a = 1;
    unsigned int add = (unsigned int)&a;
    emit signalData(a,add);
    a = 12331;
}

void Widget::recDataDirectConnection(const int &a, unsigned int aAddress)
{
    if(a != 1)
    {
        ui->textBrowser->append("a != 1");
    }
    if(aAddress != (unsigned int)&a)
    {
        ui->textBrowser->append(QString("not same,rec a:%1,thread a:%2").arg((unsigned int)&a).arg(aAddress));
    }
    else
    {
        ui->textBrowser->append(QString("!!!!! same,rec a:%1,thread a:%2").arg((unsigned int)&a).arg(aAddress));
    }
}

void Widget::recThreadData(const int &a, unsigned int aAddress)
{
    if(a != 1)
    {
        ui->textBrowser->append("a != 1");
    }
    if(aAddress != (unsigned int)&a)
    {
        ui->textBrowser->append(QString("thread not same,rec a:%1,thread a:%2").arg((unsigned int)&a).arg(aAddress));
    }
    else
    {
        ui->textBrowser->append(QString("!!!!!thread same,rec a:%1,thread a:%2").arg((unsigned int)&a).arg(aAddress));
    }

}

void Widget::recDataQueuedConnection(const int &a, unsigned int aAddress)
{
    if(a != 1)
    {
        ui->textBrowser->append("a != 1");
    }
    if(aAddress != (unsigned int)&a)
    {
        ui->textBrowser->append(QString("not same,rec a:%1,thread a:%2").arg((unsigned int)&a).arg(aAddress));
    }
    else
    {
        ui->textBrowser->append(QString("!!!!! same,rec a:%1,thread a:%2").arg((unsigned int)&a).arg(aAddress));
    }
}
#define READ_TEMP_BUF_SIZE  1024
void Widget::readData(QByteArray simulateData)
{
    //TODO
    static int read_offset = 0;
    char buf_r[READ_TEMP_BUF_SIZE];
    int nread, read_index;
Q_ASSERT_X(read_offset < READ_TEMP_BUF_SIZE, __FUNCTION__ , "read_offset must smaller than READ_TEMP_BUF_SIZE");

    //nread = read(receiveFD, buf_r + read_offset, READ_TEMP_BUF_SIZE - read_offset);
    nread = simulateData.size();
    for(int i=0;i<nread;++i)
    {
        buf_r[read_offset+i] = simulateData[i];
    }

    if(nread <= 0)
    {
        qDebug("Cannot read data %s %d! nread=%d\n", __FILE__, __LINE__, nread);
        return;
    }

    if(read_offset != 0)
    {
        for(int j = 0; j < read_offset; j++)
        {
Q_ASSERT_X(j < receiveData.size(), __FUNCTION__ , "j more than receiveData size");
            buf_r[j] = receiveData[j];
        }
        nread += read_offset;
        receiveData.resize(0);
        read_offset = 0;
    }

    read_index = 0;
    do
    {
        //determine first receive cmd data
        if(receiveData.isEmpty())
        {//czy:按照正常流程进入这个if是第一次收到数据的情况
            int i;
            //find the cmd head-- 0x55 0x55
            for(i = read_index; i < nread - 1; i++)
            {
                if(buf_r[i] == 0x55 && buf_r[i+1] == 0x55)
                    break;
                else//add by czy
                {
                    qDebug("%s,%s[%d]unexpect header,not 0x55,0x55",__FILE__,__FUNCTION__,__LINE__);
                }
            }//czy:按照正常流程此时read_index=0,i=0,nread=8
            if(nread - i < 4)
            {//czy:此逻辑的原因？进入此if是i=5也就是0x55在第5位才会
                //czy:此操作说明接收第一帧前两个不是0x55，而是后面几个才是0x55
                read_offset = nread - i;
                receiveData.resize(read_offset);
                for(int j = 0; i < nread; i++, j++)
                    receiveData[j] = buf_r[i];
                return;
            }

            read_index = i;//czy:按照正常流程此时read_index=0,i=0,nread=8

            unsigned short cmd_length = buf_r[read_index + 2];//读取长度位低位
            cmd_length |= ((buf_r[read_index + 3]<<8) & 0xff00);//读取长度位
qDebug("recv length:%d",cmd_length);
            receiveData.resize(cmd_length + 8);
            receiveIndex = 0;
        }

        for(; read_index < nread && receiveIndex < (int)receiveData.size(); read_index++)
            receiveData[receiveIndex++] = buf_r[read_index];

        //determine receive cmd done!
        if(receiveIndex >= (int)receiveData.size())
        {
            //czy:没有进行0xaa的结尾判断和校验位判断
            //add by czy
            if(receiveData.size() < 8)
            {
                //最小最小也会有8字节，如果没有就要抛弃
                receiveData.clear();
                read_offset = 0;
            }
            const int recSize = receiveData.size();
            if((unsigned char)receiveData[recSize - 1] != 0xaa
                    && (unsigned char)receiveData[recSize - 2] != 0xaa )
            {
qDebug("unexpect package:");
                receiveData.clear();
                read_offset = 0;
            }
            //end add

//            FifoData cmd;//原来的pCmd内存泄漏
//            QDataStream in(receiveData);
//            in.setByteOrder(QDataStream::LittleEndian);
//            in.setVersion(5);

//            in >> cmd;
            /*
            qDebug("receive %d:",receiveData.length());
            for(int i = 0; i < receiveData.length();i++)
            {
                unsigned char a = receiveData[i];
                qDebug("%x ",a);
            }
*/
//            if(cmd.classID != 0)
//            {
//                emit receiveCommand(cmd);
//                //qDebug("        4 receive write fifo ok cid=%d fid=%d",  pCmd->getClassID(), pCmd->getFunID());
//            }
//            else
//            {
//                qDebug("Receive wrong command %s %d!", __FILE__, __LINE__);
//                //qDebug("receive %d:",receiveData.length());
//            }
            QString str;
            for(int i=0;i<receiveData.size();++i)
            {
                str.append(QString(",0x%1").arg((unsigned char)receiveData[i],0,16));
            }
            ui->textBrowser->append(str);

            receiveData.resize(0);
            receiveIndex = 0;

        }
    }while(read_index < nread);

    return;
}


