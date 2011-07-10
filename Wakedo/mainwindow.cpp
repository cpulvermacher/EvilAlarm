#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QGraphicsObject>
#if defined(Q_WS_MAEMO)
#include <alarmd/alarm_event.h>
#endif


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString path=QApplication::applicationDirPath()+"/qml/Wakedo/main.qml";
    //bool r = QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    ui->view->setSource(QUrl::fromLocalFile(path));
    QGraphicsObject *item = ui->view->rootObject();

    QObject::connect(item, SIGNAL(selectAlarmType()),
                     this, SLOT(showSelector()));

    /*
    // trying http://doc.qt.nokia.com/qt-maemo-4.6/maemo5-stackedwindows.html
    button1.setText("About");
    //ui->layout->addWidget(&button1);
    ("Click here to open a new stacked window");
    QPushButton button1("Click here to open a new stacked window");
    //not compatible with harmattan
    button1.setAttribute(Qt::WA_Maemo5StackedWindow);

    QPushButton button2("Click here to open another stacked window", &button1);
    button2.setAttribute(Qt::WA_Maemo5StackedWindow);
    button2.setWindowFlags(button2.windowFlags() | Qt::Window);

    QLabel label("<p align=\"center\">This is the last stacked window. "
                 "Press the back arrow on the top-right corner to go back.",
                 &button2, Qt::Window);
    label.setAttribute(Qt::WA_Maemo5StackedWindow);
    label.setWordWrap(true);

    QObject::connect(&button1, SIGNAL(clicked()), &button2, SLOT(show()));
    QObject::connect(&button2, SIGNAL(clicked()), &label, SLOT(show()));
*/
    //button1.show();
    //setAttribute(Qt::WA_Maemo5LandscapeOrientation, true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::showSelector() {
    qDebug() << "Called the C++ slot.";
    selectAlarmType.show();
}


/*void MainWindow::setAlarm(int hours,int minutes){
    // from http://www.tardigrada.hr/blog/2010/10/using-maemos-alarm-framework-with-qt-the-basics/


        alarm_event_t * event = 0;
        alarm_action_t * act = 0;

        // Setup
        event = alarm_event_create();
        // The application id field is needed for every alarm you set
        alarm_event_set_alarm_appid(event, "Alarm testing app");
        // Alarm description displayed when the alarm is triggered
        alarm_event_set_message(event, "The alarm message for the system UI");
        // Set the alarm time 30s form now
        event->alarm_time = time(0) + 30;

        // Acknowledge action
        act = alarm_event_add_actions(event, 1);
        alarm_action_set_label(act, "Stop");
        act->flags |= ALARM_ACTION_WHEN_RESPONDED;
        act->flags |= ALARM_ACTION_TYPE_NOP;

        // Snooze action
        act = alarm_event_add_actions(event, 1);
        alarm_action_set_label(act, "Snooze");
        act->flags |= ALARM_ACTION_WHEN_RESPONDED;
        act->flags |= ALARM_ACTION_TYPE_SNOOZE;

        // Pass the structure to the alarmd daemon
        alarmd_event_add(event);
        // Cleanup
        alarm_event_delete(event);
        act = 0;
        event = 0;


}
*/
void MainWindow::on_actionSettings_triggered()
{

}

void MainWindow::on_actionAbout_triggered()
{
    button1.click();
}
