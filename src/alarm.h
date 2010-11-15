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
#ifndef ALARM_H
#define ALARM_H

#include "accelerometer.h"

#include <QtGui>

//default values
const int ACCELEROMETER_POLL_MSEC = 500;
const int ACCELEROMETER_THRESHOLD = 40;
const int INACTIVITY_TIMEOUT = 4; //seconds before noise will restart after device has stopped moving
const int ALARM_TIMEOUT = 5; //minutes the device is armed after initial alarm


class Alarm : public QDialog {
	Q_OBJECT
public:
	Alarm(QWidget *parent = 0, bool testing = false);
	~Alarm();
	static void test(QWidget *parent, int al_timeout, int in_timeout);
protected:
	void closeEvent(QCloseEvent*);
	void initialize();
	void start();
	void stop();
private slots:
	void accelUpdate(int, int, int);
	void repeatSound();
private:
	QLabel *label;
	Accelerometer *accel;
	bool testing;
	QTime alarm_started;
	bool alarm_playing;
	int alarm_timeout, inactivity_timeout;
	quint32 notify_id;
};
#endif
