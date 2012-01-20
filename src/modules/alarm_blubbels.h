/*
    EvilAlarm
    Copyright (C) 2010-2011 Christian Pulvermacher

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
#ifndef ALARM_BLUBBELS_H
#define ALARM_BLUBBELS_H

#include "alarm.h"
#include "blubbels.h"

#include <QtGui>

class AlarmBlubbels : public Alarm
{
    Q_OBJECT
public:
    explicit AlarmBlubbels(QWidget *parent = 0);
    ~AlarmBlubbels();
protected slots:
    void checkScore(int);
    void onActivity();
    void updateScreen();
    virtual void restart();
    void snooze();
private:
    QLabel *label;
    QPushButton *snooze_button;
    QPushButton *stop_button;
    GameWidget gamewidget;

    int inactivity_timeout;
    QTime last_active;
};
#endif
