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

#include <QtDBus>
#include <QtGui>


Alarm::Alarm(QWidget *parent, bool testing):
	QDialog(parent),
	label(new QLabel(this)),
	testing(testing),
	alarm_playing(false),
	lastx(0),
	lasty(0),
	lastz(0),
	last_active(QTime::currentTime())
{
	setWindowTitle("EvilAlarm");
	if(!testing) {
		setWindowState(windowState() | Qt::WindowFullScreen);

		//load settings
		QSettings settings;
		alarm_timeout = settings.value("alarm_timeout", ALARM_TIMEOUT).toInt();
		inactivity_timeout = settings.value("inactivity_timeout", INACTIVITY_TIMEOUT).toInt();

		//tell daemon to restart us if we're killed
		settings.setValue("protect_ui", true);
		settings.sync();
	}

	//setup ui
	QHBoxLayout* layout1 = new QHBoxLayout();
	QLabel *icon_label = new QLabel(this);
	icon_label->setPixmap(QPixmap("/usr/share/icons/hicolor/64x64/apps/evilalarm.png"));
	icon_label->setMaximumWidth(70);
	layout1->addWidget(icon_label);
	layout1->addWidget(label);
	setLayout(layout1);

	if(!testing)
		initialize();
}

//actually starts the alarm, needs to be called manually when testing
//TODO use additional constructor arguments for testing and remove this
void Alarm::initialize()
{
	start();
	alarm_started = QTime::currentTime();

	accel = new Accelerometer(this, ACCELEROMETER_POLL_MSEC);
	connect(accel, SIGNAL(orientationChanged(int, int, int)),
		this, SLOT(accelUpdate(int, int, int)));

	//activate display
	QDBusInterface interface("com.nokia.mce", "/com/nokia/mce/request", "com.nokia.mce.request", QDBusConnection::systemBus());
	interface.call("req_tklock_mode_change", "unlocked");
}

Alarm::~Alarm()
{
	delete accel;
}

void Alarm::accelUpdate(int x, int y, int z)
{
	if(alarm_started.elapsed()/1000 > alarm_timeout*60) {
		close();
		return;
	}

	if(qAbs(lastx - x) > ACCELEROMETER_THRESHOLD
	or qAbs(lasty - y) > ACCELEROMETER_THRESHOLD
	or qAbs(lastz - z) > ACCELEROMETER_THRESHOLD) { //device moved
		stop();
		last_active.restart();
	} else if(last_active.elapsed()/1000 > inactivity_timeout) { //not moved for a while
		start();
	}

	QString remaining_string;
	int secs_remaining = alarm_timeout*60 - alarm_started.elapsed()/1000;
	if(secs_remaining < 60) {
		remaining_string = tr("%1 seconds remaining").arg(secs_remaining);
	} else {
		int mins_remaining = qRound(secs_remaining/60);
		remaining_string = tr("%1 minutes remaining").arg(mins_remaining);
	}
	label->setText(tr("<center><h1>%1</h1>").arg(QTime::currentTime().toString(Qt::SystemLocaleShortDate)) + remaining_string + "</center>");

	lastx = x; lasty = y; lastz = z;
}

void Alarm::closeEvent(QCloseEvent*)
{
	hide();
	stop();

	QSettings settings;
	settings.setValue("protect_ui", false);
	settings.sync();
}


void Alarm::test(QWidget *parent, int al_timeout, int in_timeout)
{
	Alarm *alarm = new Alarm(parent, true);
	alarm->alarm_timeout = al_timeout;
	alarm->inactivity_timeout = in_timeout;

	alarm->initialize();
	alarm->exec();

	delete alarm;
}

void Alarm::start()
{
	if(alarm_playing)
		return;

	QDBusInterface interface("org.freedesktop.Notifications", "/org/freedesktop/Notifications", "org.freedesktop.Notifications");
	QVariantMap hints;
	hints.insert("vibra", "PatternIncomingCall");
	hints.insert("alarm-type", "clock"); //necessary for repeat
	//hints.insert("sound-file", ...); //ignored
	hints.insert("category", "alarm-event");
	//hints.insert("volume", qRound(max_volume*100)); //ignored

	QDBusReply<quint32> dbus_reply = interface.call("Notify",
		"evilalarm", //app_name
		quint32(0), //replaces_id
		"", //app_icon
		"alarm", //summary
		"", //body
		QStringList(), //actions
		hints, //hints
		qint32(0) //expire_timeout
	);
	Q_ASSERT(dbus_reply.isValid());

	notify_id = dbus_reply.value();
	alarm_playing = true;
}

void Alarm::stop()
{
	if(!alarm_playing)
		return;

	QDBusInterface interface("org.freedesktop.Notifications", "/org/freedesktop/Notifications", "org.freedesktop.Notifications");
	interface.call("CloseNotification", notify_id);
	alarm_playing = false;
}
