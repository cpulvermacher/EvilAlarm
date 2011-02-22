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

#include <QtGui>

class Backend;

//abstract base class for alarm modules
class Alarm : public QDialog {
	Q_OBJECT
public:
	Alarm(QWidget *parent);
	virtual ~Alarm() = 0;
protected:
	virtual void closeEvent(QCloseEvent*);
protected slots:
	virtual void deviceLockChanged(QString mode);
	virtual void restart();
	virtual void snooze();
protected:
	Backend *backend;
	QTime alarm_started;
	int alarm_timeout;
	bool snoozing;
	QTime snooze_till;
	int num_snooze;
};
#endif
