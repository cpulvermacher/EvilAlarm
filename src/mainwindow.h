#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

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
    void showAlarmHistory();
    void setEvilAlarm(int hours, int minutes);
    void unsetEvilAlarm();
private slots:
    void on_actionSettings_triggered();
    void on_actionAbout_triggered();
    void on_actionTest_Alarm_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
