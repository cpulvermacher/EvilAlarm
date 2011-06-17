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
const int ACCELEROMETER_POLL_MSEC = 200;
const int ACCELEROMETER_THRESHOLD = 50;
const QString SOUND_FILE = "/home/user/MyDocs/.sounds/Ringtones/Beep.aac";
const float MAX_VOLUME = 80;
const int INACTIVITY_TIMEOUT = 4; //seconds before noise will restart after device has stopped moving
const int ALARM_TIMEOUT = 5; //minutes the device is armed after initial alarm
const int SNOOZE_TIME = 10; //minutes
const int NUM_SNOOZE_MAX = 1;
const bool FULLSCREEN = false;
const bool USE_VIBRATION = false;


class Settings : public QScrollArea {
	Q_OBJECT
public:
	Settings(QWidget *parent);
	void save();
protected:
	void closeEvent(QCloseEvent*);
private slots:
	void pickSoundFile();
private:
	QSettings settings;
	QStackedLayout *module_settings_layout;
	QComboBox *module;
	QMaemo5ValueButton *sound_filename;
	QSlider *volume;
	QSpinBox *alarm_timeout;
	QSpinBox *inactivity_timeout;
	QSpinBox *snooze_time;
	QSpinBox *num_snooze_max;
	QCheckBox *fullscreen;
	QCheckBox *prevent_device_lock;
	QCheckBox *vibration;
};
#endif
