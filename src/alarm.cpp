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
#include "alarm_movement.h"
#include "backend.h"
#include "settings.h"

#include <QtDBus>
#include <QtGui>


Alarm::Alarm(QWidget *parent):
	QDialog(parent),
	backend(new Backend()),
	num_snooze(0)
{
	QSettings settings;
	if(parent == 0)  { //top level window?
		if(settings.value("fullscreen", FULLSCREEN).toBool())
			setWindowState(windowState() | Qt::WindowFullScreen);
		else
			setWindowFlags(Qt::Window); //allow multitasking
	}

	alarm_timeout = settings.value("alarm_timeout", ALARM_TIMEOUT).toInt();
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
	backend->setVolume(100); //set volume to the configured maximum
	alarm_started = QTime::currentTime();
	
	//activate display
	QDBusInterface interface("com.nokia.mce", "/com/nokia/mce/request", "com.nokia.mce.request", QDBusConnection::systemBus());
	interface.call("req_tklock_mode_change", "unlocked");
}


void Alarm::closeEvent(QCloseEvent* ev)
{
	if(parent() == 0 and ev->spontaneous()) {
		//user tried to close window
		ev->ignore();
		return;
	}

	hide();
	backend->pause();
}

//turns of alarm for snooze_time minutes, then restarts alarm
void Alarm::snooze()
{
	snoozing = true;
	num_snooze++;

	backend->pause();

	QSettings settings;
	const int snooze_time = settings.value("snooze_time", SNOOZE_TIME).toInt();
	const int snooze_time_msecs = snooze_time * 60 * 1000;

	QTimer::singleShot(snooze_time_msecs, this, SLOT(restart()));
}


//get the user-configured module
Alarm* Alarm::getModuleInstance(QWidget *parent)
{
	//well, maybe later
	return new AlarmMovement(parent);
}
