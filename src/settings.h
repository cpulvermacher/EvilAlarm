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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtGui>
#include <QMaemo5ValueButton>

//default values
const int ACCELEROMETER_POLL_MSEC = 500;
const int ACCELEROMETER_THRESHOLD = 50;
const int INACTIVITY_TIMEOUT = 4; //seconds before noise will restart after device has stopped moving
const int ALARM_TIMEOUT = 5; //minutes the device is armed after initial alarm
const int SNOOZE_TIME = 10; //minutes
const int NUM_SNOOZE_MAX = 1;
const bool FULLSCREEN = false;


class Settings : public QWidget {
	Q_OBJECT
public:
	Settings(QWidget *parent);
protected:
	void closeEvent(QCloseEvent*);
private:
	void save();
	QSettings settings;
	QSpinBox *alarm_timeout;
	QSpinBox *inactivity_timeout;
	QSpinBox *snooze_time;
	QSpinBox *num_snooze_max;
	QCheckBox *fullscreen;
};
#endif
