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
#include "alarm_movement.h"
#include "backend.h"
#include "settings.h"

#include <QtDBus>
#include <QtGui>

#include <iostream>

AlarmMovement::AlarmMovement(QWidget *parent):
	Alarm(parent),
	label(new QLabel(this)),
	accel(0)
{
	setWindowTitle("EvilAlarm");

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

	//actually start alarm
	restart();

	//refresh UI every second
	QTimer *ui_timer = new QTimer(this);
	connect(ui_timer, SIGNAL(timeout()),
		this, SLOT(updateScreen()));
	ui_timer->start(1000);

	//TODO: remove
//	QTimer::singleShot(1000, this, SLOT(close()));
}

//starts/restarts the alarm
void AlarmMovement::restart()
{
	accel = new Accelerometer(this, ACCELEROMETER_POLL_MSEC);
	connect(accel, SIGNAL(orientationChanged(int, int, int)),
		this, SLOT(accelUpdate(int, int, int)));
	lastx = lasty = lastz = 0;

	QSettings settings;
	inactivity_timeout = settings.value("inactivity_timeout", INACTIVITY_TIMEOUT).toInt();
	const int num_snooze_max = settings.value("num_snooze_max", NUM_SNOOZE_MAX).toInt();
	if(num_snooze_max == 0) {
		//snooze completely disabled
		snooze_button->setVisible(false);
	} else {
		snooze_button->setEnabled(num_snooze < num_snooze_max);
		snooze_button->setText(tr("Snooze (%1/%2)").arg(num_snooze).arg(num_snooze_max));
	}

	Alarm::restart();
}

AlarmMovement::~AlarmMovement()
{
	delete accel;
}

void AlarmMovement::accelUpdate(int x, int y, int z)
{
	if(snoozing) {
		std::cout << "this shouldnt hpapen\n";
		return;
	}

	//shutdown time reached?
	if(alarm_started.elapsed()/1000 > alarm_timeout*60) {
		close();
		return;
	}

	int max_diff = qMax(qAbs(lastx - x), qMax(qAbs(lasty - y), qAbs(lastz - z)));
	if(lastx == 0 and lasty == 0 and lastz == 0) {
		//initialize
		last_active.restart();
	} else if(max_diff > ACCELEROMETER_THRESHOLD and !backend->isVibrating()) {
		//device moved
		backend->volumeDown();
		last_active.restart();
	} else if(last_active.elapsed()/1000 > inactivity_timeout and !backend->isVibrating()) {
		//not moved for a while
		backend->volumeUp();
	}

	if(!backend->isVibrating()) //ignore huge spikes
		lastx = x; lasty = y; lastz = z;
}

void AlarmMovement::updateScreen()
{
	QString label_text = tr("<center><h1>%1</h1>").arg(QTime::currentTime().toString(Qt::SystemLocaleShortDate));

	//display remaining alarm or snooze time
	int secs_remaining;
	if(snoozing) {
		secs_remaining = QTime::currentTime().secsTo(snooze_till);
		label_text += tr("Snooze: ");
	} else {
		secs_remaining = alarm_timeout*60 - alarm_started.elapsed()/1000;
	}

	if(secs_remaining >= 60) {
		label_text += tr("%1 minutes, ", "", secs_remaining/60).arg(secs_remaining/60);
	}
	label_text += tr("%1 seconds remaining</center>", "", secs_remaining%60).arg(secs_remaining%60);

	label->setText(label_text);
}

void AlarmMovement::snooze()
{
	snooze_button->setEnabled(false);

	Alarm::snooze();

	//stop polling
	delete accel;
	accel = 0;
}
