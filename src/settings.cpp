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
#include "settings.h"

#include <QtGui>


Settings::Settings(QWidget *parent):
	QWidget(parent)
{
	setWindowTitle(tr("Settings"));
	setAttribute(Qt::WA_Maemo5StackedWindow);
	setWindowFlags(windowFlags() | Qt::Window); //create top-level window even though parent is set
	//for reference: Qt::WA_Maemo5StackedWindow doesn't seem to work with QDialog

	QGridLayout *layout = new QGridLayout(this);
	layout->setColumnStretch(0, 1);

	QLabel *alarm_timeout_label = new QLabel(tr("Completely shut down after"));
	alarm_timeout = new QSpinBox();
	alarm_timeout->setSuffix(" min");
	alarm_timeout->setRange(1, 60);
	alarm_timeout->setValue(settings.value("alarm_timeout", ALARM_TIMEOUT).toInt());
	layout->addWidget(alarm_timeout_label, 0, 0);
	layout->addWidget(alarm_timeout, 0, 1);

	QLabel *inactivity_timeout_label = new QLabel(tr("Restart alarm if phone not moved for"));
	inactivity_timeout = new QSpinBox();
	inactivity_timeout->setSuffix(" s");
	inactivity_timeout->setValue(settings.value("inactivity_timeout", INACTIVITY_TIMEOUT).toInt());
	layout->addWidget(inactivity_timeout_label, 1, 0);
	layout->addWidget(inactivity_timeout, 1, 1);

	QLabel *snooze_time_label = new QLabel(tr("Snooze time"));
	snooze_time = new QSpinBox();
	snooze_time->setSuffix(" min");
	snooze_time->setRange(1, 60);
	snooze_time->setValue(settings.value("snooze_time", SNOOZE_TIME).toInt());
	layout->addWidget(snooze_time_label, 2, 0);
	layout->addWidget(snooze_time, 2, 1);

	QLabel *num_snooze_max_label = new QLabel(tr("Max. number of snoozes (0 to disable snooze)"));
	num_snooze_max = new QSpinBox();
	num_snooze_max->setValue(settings.value("num_snooze_max", NUM_SNOOZE_MAX).toInt());
	layout->addWidget(num_snooze_max_label, 3, 0);
	layout->addWidget(num_snooze_max, 3, 1);

	fullscreen = new QCheckBox(tr("Full screen - No other apps (including Phone app) can be used until alarm shuts down"));
	fullscreen->setChecked(settings.value("fullscreen", FULLSCREEN).toBool());
	layout->addWidget(fullscreen, 4, 0, 1, 2);
}

void Settings::closeEvent(QCloseEvent*)
{
	save();
	deleteLater();
}

void Settings::save()
{
	settings.setValue("alarm_timeout", alarm_timeout->value());
	settings.setValue("inactivity_timeout", inactivity_timeout->value());
	settings.setValue("snooze_time", snooze_time->value());
	settings.setValue("num_snooze_max", num_snooze_max->value());
	settings.setValue("fullscreen", fullscreen->isChecked());
	settings.sync();
}
