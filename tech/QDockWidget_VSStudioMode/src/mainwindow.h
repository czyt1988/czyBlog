#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void removeAllDock();
    void showDock(const QList<int>& index = QList<int>());
private slots:
    void on_action_dock1_triggered();

    void on_action_removeAll_triggered();

    void on_action_splitDockFun_triggered();

    void on_action_addDockWidget_triggered();

    void on_action_dock2_triggered();

    void on_action_dock3_triggered();

    void on_action_tabifyDockWidget_triggered();

private:
    Ui::MainWindow *ui;
    QList<QDockWidget*> m_docks;
};

#endif // MAINWINDOW_H
