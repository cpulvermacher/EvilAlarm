#include "mainwindow.h"
#include "selectalarmtype.h"
#include "alarmhistory.h"
#include "settings.h"
#include "about.h"
#include "daemon.h"

#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QDBusConnection>
#include <QSettings>
#include <QGraphicsObject>
#include <QFile>
#include <QMaemo5InformationBox>

#include <mce/mode-names.h>
#include <mce/dbus-names.h>

#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    view(new QDeclarativeView(this))
{
    setWindowTitle("EvilAlarm");
    setAttribute(Qt::WA_Maemo5StackedWindow);

    menuBar()->addAction(tr("Settings"), this, SLOT(showSettings()));
    menuBar()->addAction(tr("About"), this, SLOT(showAbout()));
    menuBar()->addAction(tr("Test Alarm"), this, SLOT(testAlarm()));

    QString path;
    const QString maemo_install_path("/opt/evilalarm/qml/main.qml");
    if(QFile::exists(maemo_install_path)) {
        path = maemo_install_path;
    } else { //for QtCreator
        path = QApplication::applicationDirPath()+"/../qml/qml/main.qml";
    }

    //avoid flicker at start
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    view->setAttribute(Qt::WA_OpaquePaintEvent);
    view->setAttribute(Qt::WA_NoSystemBackground);

    //now load UI
    view->setSource(QUrl::fromLocalFile(path));
    QGraphicsObject *root_object = view->rootObject();


    //load alarm
    QSettings settings;
    const QTime alarm_time = settings.value("wake_at", QTime::currentTime()).toTime();
    root_object->setProperty("non_user_action", true);
    root_object->setProperty("ui_alarm_hours", alarm_time.hour());
    root_object->setProperty("ui_alarm_minutes", alarm_time.minute());
    root_object->setProperty("ui_alarm_active", Daemon::isRunning());
    root_object->setProperty("ui_alarm_type", settings.value("module", "Normal").toString());
    root_object->setProperty("non_user_action", false);


    QObject::connect(root_object, SIGNAL(selectAlarmType()),
                     this, SLOT(showAlarmTypeSelector()));
    QObject::connect(root_object, SIGNAL(showAlarmHistory(int, int)),
                     this, SLOT(showAlarmHistory(int, int)));
    QObject::connect(root_object, SIGNAL(unsetAlarm()),
                     this, SLOT(unsetEvilAlarm()));
    QObject::connect(root_object, SIGNAL(setAlarm(int, int)),
                     this, SLOT(setEvilAlarm(int, int)));
    QDBusConnection::systemBus().connect("", MCE_SIGNAL_PATH, MCE_SIGNAL_IF, MCE_DISPLAY_SIG,
                                         this, SLOT(displayStateChanged(QString)));

    if(!settings.isWritable()) {
        QMaemo5InformationBox::information(this, tr("Can't write settings to /home/user/.config/EvilAlarm/EvilAlarm - Please fix its file permissions and restart EvilAlarm"));
    }

    const QString file_path("/opt/evilalarm/bin/keepvolume.sh");
    if(!QFile::permissions(file_path).testFlag(QFile::ExeOther)) {
        QMaemo5InformationBox::information(this, tr("Can't execute %1 - Please reinstall EvilAlarm!").arg(file_path));
    }
}

MainWindow::~MainWindow()
{
    delete view;
}

void MainWindow::showAlarmTypeSelector()
{
    SelectAlarmType* selectAlarmType = new SelectAlarmType(this);
    selectAlarmType->exec();

    QGraphicsObject *root_object = view->rootObject();
    QSettings settings;
    root_object->setProperty("ui_alarm_type", settings.value("module", "Normal").toString());

    delete selectAlarmType;
}


void MainWindow::setEvilAlarm(int hours, int minutes)
{
    std::cout << "setEvilAlarm(" << hours << ", " << minutes << ")\n";
    QSettings settings;

    const QTime wake_at(hours, minutes);

    if(Daemon::isRunning()) {
        if(settings.value("wake_at").toTime() == wake_at)
            return; //time didn't change, nothing to do

        //new alarm time, discard old one
        unsetEvilAlarm();
    }

    settings.setValue("wake_at", wake_at);

    //save to history
    settings.beginGroup("history");
    const int num_used = settings.value(QString("%1/used").arg(wake_at.toString()), 0).toInt();
    settings.setValue(QString("%1/used").arg(wake_at.toString()), num_used+1);
    settings.endGroup();

    settings.sync();

    Daemon::start();
}

void MainWindow::unsetEvilAlarm()
{
    std::cout << "unsetEvilAlarm()\n";
    Daemon::stop();

    //since unsetEvilAlarm() is only called when the user deactivates a set alarm, remove the most recent alarm from history
    QSettings settings;
    const QString wake_at_string = QString("%1/used").arg(settings.value("wake_at").toTime().toString());
    settings.beginGroup("history");
    const int num_used = settings.value(wake_at_string, 0).toInt();
    if(num_used == 0) {
        std::cerr << "trying to remove nonexistant alarm from history\n";
        return;
    } else if(num_used == 1) {
        settings.remove(wake_at_string);
    } else {
        settings.setValue(wake_at_string, num_used-1);
    }
    settings.endGroup();
    settings.sync();
}

void MainWindow::showSettings()
{
    Settings *settingsWindow = new Settings(this);
    settingsWindow->setAttribute(Qt::WA_Maemo5StackedWindow);
    settingsWindow->setWindowFlags(windowFlags() | Qt::Window);
    settingsWindow->exec();
    delete settingsWindow;

    QGraphicsObject *root_object = view->rootObject();
    QSettings settings;
    root_object->setProperty("ui_alarm_type", settings.value("module", "Normal").toString());
}

void MainWindow::showAbout()
{
    static About *about = new About(this);
    about->setAttribute(Qt::WA_Maemo5StackedWindow);
    about->setWindowFlags(windowFlags() | Qt::Window);
    about->show();
}

void MainWindow::testAlarm()
{
    QProcess::startDetached(QString("evilalarm --test"));
}

void MainWindow::displayStateChanged(QString state)
{
    const bool display_on = (state != "off");
    QGraphicsObject *root_object = view->rootObject();
    root_object->setProperty("displayOn", display_on);
}

void MainWindow::showAlarmHistory(int hours, int minutes)
{
    AlarmHistory* alarmHistory = new AlarmHistory(this, hours, minutes);
    connect(alarmHistory, SIGNAL(setAlarm(int, int)),
            this, SLOT(setUIAlarm(int, int)));
    alarmHistory->exec();
    delete alarmHistory;
}

//set alarm in UI (onChanged handlers are triggered)
void MainWindow::setUIAlarm(int hours, int minutes)
{
    QGraphicsObject *root_object = view->rootObject();
    //disable old alarm first
    //(changing hours / minutes to something else is required to trigger the onChanged slots...)
    root_object->setProperty("ui_alarm_active", false);
    root_object->setProperty("ui_alarm_hours", -1);
    root_object->setProperty("ui_alarm_minutes", -1);

    root_object->setProperty("ui_alarm_hours", hours);
    root_object->setProperty("ui_alarm_minutes", minutes);
    root_object->setProperty("ui_alarm_active", true);
}
