#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QPushButton>
#include <QMainWindow>
#include "selectalarmtype.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //void setAlarm(int hours,int minutes);

public slots:
    void showSelector();
private slots:
    void on_actionSettings_triggered();

    void on_actionAbout_triggered();

private:
    SelectAlarmType selectAlarmType;
    Ui::MainWindow *ui;
    QPushButton button1;
};

#endif // MAINWINDOW_H
