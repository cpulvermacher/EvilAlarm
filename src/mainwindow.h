#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QString>

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
    //(de)activate alarm (i.e. start/stop daemon)
    void setEvilAlarm(int hours, int minutes);
    void unsetEvilAlarm();

    //dialog to select alarm time
    void showAlarmTypeSelector();

    //alarm history dialog, with option to add currently set time to favorites
    void showAlarmHistory(int hours, int minutes);

    //used for setting alarm from history, changes UI state directly (which in turn calls setEvilAlarm())
    void setUIAlarm(int hours, int minutes);

    //other dialogs
    void on_actionSettings_triggered();
    void on_actionAbout_triggered();
    void on_actionTest_Alarm_triggered();

    //if display is off, stop all animations
    void displayStateChanged(QString state);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
