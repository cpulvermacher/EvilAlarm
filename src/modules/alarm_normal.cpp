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
#include "alarm_normal.h"
#include "settings.h"

#include <QtGui>


AlarmNormal::AlarmNormal(QWidget *parent, bool test):
    Alarm(parent, test),
    label(new QLabel(this)),
    snooze_button(new QPushButton(this)),
    stop_button(new QPushButton(this))
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

    QHBoxLayout* layout2 = new QHBoxLayout();
    layout2->addWidget(snooze_button);
    stop_button->setText(tr("Stop Alarm"));
    layout2->addWidget(stop_button);

    layout0->addLayout(layout1);
    layout0->addLayout(layout2);
    setLayout(layout0);

    connect(snooze_button, SIGNAL(clicked()),
            this, SLOT(snooze()));
    connect(stop_button, SIGNAL(clicked()),
            this, SLOT(close()));

    //actually start alarm
    restart();

    //refresh UI every second
    QTimer *ui_timer = new QTimer(this);
    connect(ui_timer, SIGNAL(timeout()),
            this, SLOT(updateScreen()));
    ui_timer->start(1000);
}

//starts/restarts the alarm
void AlarmNormal::restart()
{
    QSettings settings;
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


void AlarmNormal::updateScreen()
{
    QString label_text = tr("<center><span style='font-size:100px;'>%1</span><br />").arg(QTime::currentTime().toString(Qt::SystemLocaleShortDate));

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

void AlarmNormal::snooze()
{
    snooze_button->setEnabled(false);
    Alarm::snooze();
}
