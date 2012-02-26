#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QString>

class QDeclarativeView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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
    void showSettings();
    void showAbout();
    void testAlarm();

    //if display is off, stop all animations
    void displayStateChanged(QString state);

private:
    QDeclarativeView *view;
};

#endif // MAINWINDOW_H
