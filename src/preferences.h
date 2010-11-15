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

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QtGui>
#include <QMaemo5ValueButton>

class Preferences : public QDialog {
	Q_OBJECT
public:
	Preferences(QWidget *parent);
	~Preferences();
private slots:
	void save();
	void testAlarm();
private:
	QSettings settings;
	//QSlider *volume;
	QSpinBox *alarm_timeout;
	QSpinBox *inactivity_timeout;
};
#endif
