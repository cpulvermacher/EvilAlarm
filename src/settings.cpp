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
#include "alarm.h"
#include "settings.h"

#include <QtGui>


Settings::Settings(QWidget *parent):
	QWidget(parent)
{
	setWindowTitle(tr("Settings"));
	setAttribute(Qt::WA_Maemo5StackedWindow);
	setWindowFlags(windowFlags() | Qt::Window); //create top-level window even though parent is set
	//for reference: Qt::WA_Maemo5StackedWindow doesn't seem to work with QDialog

	QHBoxLayout *layout1 = new QHBoxLayout();
	QVBoxLayout *layout2 = new QVBoxLayout();

	QHBoxLayout *layout4 = new QHBoxLayout();
	QLabel *alarm_timeout_label = new QLabel(tr("Completely shutdown after"));
	alarm_timeout = new QSpinBox();
	alarm_timeout->setSuffix(" min");
	alarm_timeout->setRange(1, 60);
	alarm_timeout->setValue(settings.value("alarm_timeout", ALARM_TIMEOUT).toInt());
	alarm_timeout->setMaximumWidth(250);
	layout4->addWidget(alarm_timeout_label);
	layout4->addWidget(alarm_timeout);

	QHBoxLayout *layout5 = new QHBoxLayout();
	QLabel *inactivity_timeout_label = new QLabel(tr("Restart alarm if phone not moved for"));
	inactivity_timeout = new QSpinBox();
	inactivity_timeout->setSuffix(" s");
	inactivity_timeout->setValue(settings.value("inactivity_timeout", INACTIVITY_TIMEOUT).toInt());
	inactivity_timeout->setMaximumWidth(250);
	layout5->addWidget(inactivity_timeout_label);
	layout5->addWidget(inactivity_timeout);

	QHBoxLayout *layout6 = new QHBoxLayout();
	QLabel *snooze_time_label = new QLabel(tr("Snooze time"));
	snooze_time = new QSpinBox();
	snooze_time->setSuffix(" min");
	snooze_time->setRange(1, 60);
	snooze_time->setValue(settings.value("snooze_time", SNOOZE_TIME).toInt());
	snooze_time->setMaximumWidth(250);
	layout6->addWidget(snooze_time_label);
	layout6->addWidget(snooze_time);

	QHBoxLayout *layout7 = new QHBoxLayout();
	QLabel *num_snooze_max_label = new QLabel(tr("Max. number of snoozes (0 to disable)"));
	num_snooze_max = new QSpinBox();
	num_snooze_max->setValue(settings.value("num_snooze_max", NUM_SNOOZE_MAX).toInt());
	num_snooze_max->setMaximumWidth(250);
	layout7->addWidget(num_snooze_max_label);
	layout7->addWidget(num_snooze_max);

	layout2->addLayout(layout4);
	layout2->addLayout(layout5);
	layout2->addLayout(layout6);
	layout2->addLayout(layout7);

	QPushButton *ok = new QPushButton(tr("OK"));
	QPushButton *test = new QPushButton(tr("Test"), this);
	ok->setMaximumWidth(100);
	test->setMaximumWidth(100);

	layout1->addLayout(layout2);
	layout1->addWidget(test);

	setLayout(layout1);

	connect(test, SIGNAL(clicked()),
		this, SLOT(testAlarm()));
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
	settings.sync();
}

void Settings::testAlarm()
{
	save();
	//TODO: get rid of test(), use constructor directly
	Alarm::test(this, alarm_timeout->value(), inactivity_timeout->value());
}
