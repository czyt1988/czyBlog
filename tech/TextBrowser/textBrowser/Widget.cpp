#include "Widget.h"
#include "ui_Widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(&m_timer,&QTimer::timeout,this,&Widget::on_timeOut);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    ui->textEdit->append("start");
    ui->textEdit->append("present 0%...");
    m_present = 0;
    m_timer.stop();
    m_timer.start(100);
}

void Widget::on_timeOut()
{
    ++m_present;
    if(m_present >= 100)
    {
        m_timer.stop();
    }
    ui->textEdit->moveCursor(QTextCursor::StartOfLine,QTextCursor::KeepAnchor);
    ui->textEdit->textCursor().deleteChar();
    ui->textEdit->textCursor().insertText(QString("present %1% ...").arg(m_present));
    //ui->textEdit->append(QString("present %1% ...").arg(m_present));
    //ui->textEdit->setText(QString("present %1% ...").arg(m_present));
    //ui->textEdit->append("present 0%...");
}
