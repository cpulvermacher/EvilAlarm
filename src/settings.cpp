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
	QDialog(parent)
{
	setWindowTitle(tr("Settings"));

	QHBoxLayout *layout1 = new QHBoxLayout();
	QVBoxLayout *layout2 = new QVBoxLayout();

	/*
	volume = new QSlider(Qt::Horizontal, this);
	volume->setRange(1, 16); //for some reason, volume goes to 0.16
	volume->setValue(10 * settings.value("sound_volume", VOLUME).toFloat());
	volume->setMinimumWidth(100);
	layout3->addWidget(volume);
	*/
	
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

	layout2->addLayout(layout4);
	layout2->addLayout(layout5);

	QPushButton *ok = new QPushButton(tr("OK"));
	QPushButton *test = new QPushButton(tr("Test"), this);
	ok->setMaximumWidth(100);
	test->setMaximumWidth(100);

	layout1->addLayout(layout2);
	QVBoxLayout *layout6 = new QVBoxLayout();
	layout6->addWidget(test);
	layout6->addWidget(ok);
	layout1->addLayout(layout6);

	setLayout(layout1);

	connect(test, SIGNAL(clicked()),
		this, SLOT(testAlarm()));
	connect(ok, SIGNAL(clicked()),
		this, SLOT(accept()));
	connect(this, SIGNAL(accepted()),
		this, SLOT(save()));
}

Settings::~Settings() { }

void Settings::save()
{
	settings.setValue("alarm_timeout", alarm_timeout->value());
	settings.setValue("inactivity_timeout", inactivity_timeout->value());
	settings.sync();
}

void Settings::testAlarm()
{
	Alarm::test(this, alarm_timeout->value(), inactivity_timeout->value());
}
