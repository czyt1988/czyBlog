#include <QCoreApplication>
#include <QDebug>

class normal_class
{
public:
    normal_class():m_a(123456)
    {

    }
    ~normal_class(){
        qDebug() << "normal_class destroy";
    }
    void print() const{
        qDebug() << "normal_class print";
    }
    void printMemA() const{
        qDebug() << "normal_class printMemA:"<<m_a;
    }

private:
    int m_a;
};

class Obj1 : public QObject
{
public:
    Obj1(QObject* p=nullptr):QObject(p){
        m_mormal = new normal_class();
    }
    ~Obj1(){
        qDebug() << "Obj1 destroy";
        delete m_mormal;
    }
    const normal_class& getNormalClass() const{
        return *m_mormal;
    }

private:
    normal_class* m_mormal;
};

class Obj2 : public QObject
{
public:
    Obj2(QObject* p=nullptr):QObject(p){

    }
    ~Obj2(){
        qDebug() << "Obj2 destroy";
        Obj1* p = (Obj1*)parent();
        p->getNormalClass().print();
        p->getNormalClass().printMemA();
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Obj1* obj1 = new Obj1();
    Obj2* obj2 = new Obj2(obj1);
    delete obj1;
    return a.exec();
}
