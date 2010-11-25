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
#include "settings.h"

#include <QtDBus>
#include <QtGui>


Alarm::Alarm(QWidget *parent):
	QDialog(parent),
	label(new QLabel(this)),
	accel(0),
	num_snooze(0)
{
	setWindowTitle("EvilAlarm");

	QSettings settings;
	if(!parent) { //top level window? (=not testing)
		setWindowState(windowState() | Qt::WindowFullScreen);

		//tell daemon to restart us if we're killed
		settings.setValue("protect_ui", true);
		settings.sync();
	}

	//load settings
	alarm_timeout = settings.value("alarm_timeout", ALARM_TIMEOUT).toInt();
	inactivity_timeout = settings.value("inactivity_timeout", INACTIVITY_TIMEOUT).toInt();

	//setup ui
	QVBoxLayout* layout0 = new QVBoxLayout();
	QHBoxLayout* layout1 = new QHBoxLayout();
	QLabel *icon_label = new QLabel(this);
	icon_label->setPixmap(QPixmap("/usr/share/icons/hicolor/64x64/apps/evilalarm.png"));
	icon_label->setMaximumWidth(70);
	layout1->addWidget(icon_label);
	layout1->addWidget(label);

	layout0->addLayout(layout1);
	snooze_button = new QPushButton(this);
	layout0->addWidget(snooze_button);
	setLayout(layout0);

	connect(snooze_button, SIGNAL(clicked()),
		this, SLOT(snooze()));

	initialize();
}

//starts/restarts the alarm
void Alarm::initialize()
{
	alarm_playing = false;
	snoozing = false;
	lastx = lasty = lastz = 0;

	QSettings settings;
	const int num_snooze_max = settings.value("num_snooze_max", NUM_SNOOZE_MAX).toInt();
	if(num_snooze_max == 0) {
		//snooze completely disabled
		snooze_button->setVisible(false);
	} else {
		snooze_button->setEnabled(num_snooze < num_snooze_max);
		snooze_button->setText(tr("Snooze (%1/%2)").arg(num_snooze).arg(num_snooze_max));
	}

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
	if(!snoozing) {
		//shutdown time reached?
		if(alarm_started.elapsed()/1000 > alarm_timeout*60) {
			close();
			return;
		}

		if(lastx == 0 and lasty == 0 and lastz == 0) {
			//initialize
			last_active.restart();
		} else if(qAbs(lastx - x) > ACCELEROMETER_THRESHOLD
		or qAbs(lasty - y) > ACCELEROMETER_THRESHOLD
		or qAbs(lastz - z) > ACCELEROMETER_THRESHOLD) {
			//device moved
			stop();
			last_active.restart();
		} else if(last_active.elapsed()/1000 > inactivity_timeout) {
			//not moved for a while
			start();
		}
	}
	lastx = x; lasty = y; lastz = z;

	//update UI
	QString label_text;
	label_text = tr("<center><h1>%1</h1>").arg(QTime::currentTime().toString(Qt::SystemLocaleShortDate));

	if(!snoozing) {
		int secs_remaining = alarm_timeout*60 - alarm_started.elapsed()/1000;
		if(secs_remaining < 60) {
			label_text += tr("%1 seconds remaining").arg(secs_remaining);
		} else {
			int mins_remaining = qRound(secs_remaining/60);
			label_text += tr("%1 minutes remaining").arg(mins_remaining);
		}
	} else {
		QSettings settings;
		const int snooze_time = settings.value("snooze_time", SNOOZE_TIME).toInt();
		label_text += tr("Snoozing for %1 minutes").arg(snooze_time);
	}
	label_text += "</center";

	label->setText(label_text);
}

void Alarm::closeEvent(QCloseEvent*)
{
	hide();
	stop();

	if(!parent()) {
		QSettings settings;
		settings.setValue("protect_ui", false);
		settings.sync();
	}
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

//turns of alarm for snooze_time minutes, then restarts alarm
//
void Alarm::snooze()
{
	snoozing = true;
	num_snooze++;

	stop();

	QSettings settings;
	const int snooze_time = settings.value("snooze_time", SNOOZE_TIME).toInt();
	const int snooze_time_msecs = snooze_time * 60 * 1000;

	QTimer::singleShot(snooze_time_msecs, this, SLOT(initialize()));

	snooze_button->setEnabled(false);
}
