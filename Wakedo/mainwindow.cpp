#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef EVILALARM
#include "daemon.h"
#endif

#include <QDeclarativeContext>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QSettings>
#include <QGraphicsObject>
#include <QFile>

#if defined(Q_WS_MAEMO)
#include <alarmd/alarm_event.h>
#endif



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowTitle("EvilAlarm");
    ui->setupUi(this);
    QDeclarativeContext *context = ui->view->rootContext();

    //load alarm
    QSettings settings;
    QTime alarm_time = settings.value("wake_at", QTime::currentTime()).toTime();
    context->setContextProperty("evilalarm_hours", alarm_time.hour());
    context->setContextProperty("evilalarm_minutes", alarm_time.minute());
#ifdef EVILALARM
    context->setContextProperty("evilalarm_active", Daemon::isRunning());
#endif

    QString path;
    //now load UI
    QString realMaemo=QApplication::applicationDirPath()+"/../Wakedo/qml/Wakedo/main.qml";
    if(QFile::exists(realMaemo)){
        path=realMaemo;
    }else{
        //simulator
        path=QApplication::applicationDirPath()+"/qml/Wakedo/main.qml";

    }

    //bool r = QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    ui->view->setSource(QUrl::fromLocalFile(path));
    QGraphicsObject *item = ui->view->rootObject();

    QObject::connect(item, SIGNAL(selectAlarmType()),
                     this, SLOT(showSelector()));
    QObject::connect(item, SIGNAL(unsetAlarm()),
                     this, SLOT(unsetEvilAlarm()));
    QObject::connect(item, SIGNAL(setAlarm(int, int)),
                     this, SLOT(setEvilAlarm(int, int)));
		

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

void MainWindow::setEvilAlarm(int hours,int minutes) {
#ifdef EVILALARM
		QSettings settings;

		const QTime wake_at(hours, minutes);
		settings.setValue("wake_at", wake_at);
		settings.sync();

		int msecs = QTime::currentTime().msecsTo(wake_at);
		if(msecs < 0) //alarm tomorrow?
			msecs += 24*60*60*1000; //+24h
		

		Daemon::start();
#endif
}

void MainWindow::unsetEvilAlarm() {
#ifdef EVILALARM
		Daemon::stop();
#endif
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
