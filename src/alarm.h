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

#include <Phonon>
#include <QtGui>

const int ACCELEROMETER_POLL_MSEC = 500;
const int ACCELEROMETER_THRESHOLD = 40;

class Alarm : public QDialog {
	Q_OBJECT
public:
	Alarm(QWidget *parent, bool testing = false);
	~Alarm();
	static void test(QWidget *parent, QString sound_filename, float max_vol, int al_timeout, int in_timeout);
protected:
	void closeEvent(QCloseEvent*);
private slots:
	void accelUpdate(int, int, int);
	void repeatSound();
private:
	void grabZoomKeys(bool grab);
	void start();

	QLabel *label;
	Accelerometer *accel;
	Phonon::MediaObject* noise;
	Phonon::AudioOutput* audio_output;
	QTime alarm_started;
	int alarm_timeout, inactivity_timeout;
	float max_volume;
	QString old_profile;
	bool testing;
};
#endif
