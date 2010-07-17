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
#include <QtGui>

#include "alarm.h"
#include "preferences.h"

Preferences::Preferences(QWidget *parent):
	QDialog(parent)
{
	setWindowTitle("Preferences");

	QHBoxLayout *layout1 = new QHBoxLayout();
	QVBoxLayout *layout2 = new QVBoxLayout();

	QHBoxLayout *layout3 = new QHBoxLayout();
	sound_filename = new QMaemo5ValueButton("Sound File", this);
	sound_filename->setValueText(settings.value("sound_filename", SOUND_FILE).toString());
	volume = new QSlider(Qt::Horizontal, this);
	volume->setRange(1, 16); //for some reason, volume goes to 0.16
	volume->setValue(10 * settings.value("sound_volume", VOLUME).toFloat());
	volume->setMinimumWidth(100);
	layout3->addWidget(sound_filename);
	layout3->addWidget(volume);
	
	QHBoxLayout *layout4 = new QHBoxLayout();
	QLabel *alarm_timeout_label = new QLabel("Turn off after");
	alarm_timeout = new QSpinBox();
	alarm_timeout->setSuffix(" min");
	alarm_timeout->setValue(settings.value("alarm_timeout", ALARM_TIMEOUT).toInt());
	layout4->addWidget(alarm_timeout_label);
	layout4->addWidget(alarm_timeout);

	QHBoxLayout *layout5 = new QHBoxLayout();
	QLabel *inactivity_timeout_label = new QLabel("Inactivity Timeout");
	inactivity_timeout = new QSpinBox();
	inactivity_timeout->setSuffix(" s");
	inactivity_timeout->setValue(settings.value("inactivity_timeout", INACTIVITY_TIMEOUT).toInt());
	layout5->addWidget(inactivity_timeout_label);
	layout5->addWidget(inactivity_timeout);

	layout2->addLayout(layout3);
	layout2->addLayout(layout4);
	layout2->addLayout(layout5);

	QPushButton *ok = new QPushButton("OK");
	QPushButton *test = new QPushButton("Test", this);

	layout1->addLayout(layout2);
	QVBoxLayout *layout6 = new QVBoxLayout();
	layout6->addWidget(test);
	layout6->addWidget(ok);
	layout1->addLayout(layout6);

	setLayout(layout1);

	connect(sound_filename, SIGNAL(clicked()),
		this, SLOT(pickSoundFile()));
	connect(test, SIGNAL(clicked()),
		this, SLOT(testAlarm()));
	connect(ok, SIGNAL(clicked()),
		this, SLOT(accept()));
	connect(this, SIGNAL(accepted()),
		this, SLOT(save()));
}

Preferences::~Preferences()
{

}

void Preferences::pickSoundFile()
{
	QString name = QFileDialog::getOpenFileName(this, tr("Choose Sound File"), sound_filename->valueText(), "Sound Files(*.wav *.mp3 *.ogg *.aac)");
	if(!name.isEmpty())
		sound_filename->setValueText(name);
}

void Preferences::save()
{
	settings.setValue("sound_filename", sound_filename->valueText());
	settings.setValue("sound_volume", volume->value()/10.0);
	settings.setValue("alarm_timeout", alarm_timeout->value());
	settings.setValue("inactivity_timeout", inactivity_timeout->value());
	settings.sync();
}

void Preferences::testAlarm()
{
	Alarm::test(this, sound_filename->valueText(), volume->value()/10.0, alarm_timeout->value(), inactivity_timeout->value());
}

