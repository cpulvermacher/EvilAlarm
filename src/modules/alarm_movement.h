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
#ifndef ALARM_MOVEMENT_H
#define ALARM_MOVEMENT_H

#include "accelerometer.h"
#include "alarm.h"

#include <QtGui>

class AlarmMovement : public Alarm
{
    Q_OBJECT
public:
    explicit AlarmMovement(QWidget *parent = 0, bool test = false);
    ~AlarmMovement();
protected slots:
    void accelUpdate(int, int, int);
    void updateScreen();
    virtual void restart();
    void snooze();
private:
    QLabel *label;
    QPushButton *snooze_button;

    Accelerometer *accel;
    int inactivity_timeout;
    int accel_threshold;
    int lastx, lasty, lastz;
    QTime last_active;
};
#endif
