/*
    EvilAlarm
    Copyright (C) 2010 Christian Pulvermacher

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "alarm.h"
#include "backend.h"
#include "settings.h"

#include <QtDBus>
#include <QtGui>

#include <mce/mode-names.h>
#include <mce/dbus-names.h>


Alarm::Alarm(QWidget *parent, bool test):
    QDialog(parent),
    test(test),
    backend(new Backend(this)),
    num_snooze(0)
{
    QSettings settings;
    if(parent == 0 and !test and settings.value("fullscreen", FULLSCREEN).toBool()) {
        setWindowState(windowState() | Qt::WindowFullScreen);
    } else {
        setWindowFlags(Qt::Window); //allow multitasking
    }

    alarm_timeout = settings.value("alarm_timeout", ALARM_TIMEOUT).toInt();

    if(settings.value("prevent_device_lock", false).toBool()) {
        QDBusConnection::systemBus().connect("", MCE_SIGNAL_PATH, MCE_SIGNAL_IF, MCE_DEVLOCK_MODE_SIG, this, SLOT(deviceLockChanged(QString)));
    }
}

Alarm::~Alarm()
{
    delete backend;
}

//starts/restarts the alarm
void Alarm::restart()
{
    snoozing = false;

    backend->play();
    backend->setVolume(1.0); //start with max volume
    alarm_started = QTime::currentTime();

    //activate display
    QDBusInterface interface("com.nokia.mce", "/com/nokia/mce/request", "com.nokia.mce.request", QDBusConnection::systemBus());
    interface.call("req_tklock_mode_change", "unlocked");

    //unlock device
    QSettings settings;
    if(settings.value("prevent_device_lock", false).toBool()) {
        interface.asyncCall("devlock_callback", qint32(2));
    }
}


void Alarm::closeEvent(QCloseEvent* ev)
{
    if(parent() == 0 and !test and ev->spontaneous()) {
        //user tried to close window
        ev->ignore();
        return;
    }

    hide();
    backend->pause();
}

//turns off alarm for snooze_time minutes, then restarts alarm
void Alarm::snooze()
{
    snoozing = true;
    num_snooze++;

    backend->pause();

    QSettings settings;
    const int snooze_time = settings.value("snooze_time", SNOOZE_TIME).toInt();
    const int snooze_time_msecs = snooze_time * 60 * 1000;
    snooze_till = QTime::currentTime().addMSecs(snooze_time_msecs);

    QTimer::singleShot(snooze_time_msecs, this, SLOT(restart()));
}

void Alarm::deviceLockChanged(QString mode)
{
    if(mode == "locked") {
        //unlock
        QDBusInterface interface("com.nokia.mce", "/com/nokia/mce/request", "com.nokia.mce.request", QDBusConnection::systemBus());
        interface.asyncCall("devlock_callback", qint32(2));
    }
}
