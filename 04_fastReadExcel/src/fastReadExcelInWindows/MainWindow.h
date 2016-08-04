#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QList>
#include <QVariant>
#include "ExcelBase.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_open_triggered();

    void on_action_write_triggered();

private:
    Ui::MainWindow *ui;
    QScopedPointer<ExcelBase> m_xls;
    QList< QList<QVariant> > m_datas;
};

#endif // MAINWINDOW_H
