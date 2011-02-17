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
#include "module_list.h"
#include "settings.h"

#include <QtGui>


Settings::Settings(QWidget *parent):
	QScrollArea(parent),
	module_settings(0)
{
	setWindowTitle(tr("Settings"));
	setAttribute(Qt::WA_Maemo5StackedWindow);
	setWindowFlags(windowFlags() | Qt::Window); //create top-level window even though parent is set

	//'widget' contains all actual ui elements
	QWidget *widget = new QWidget;
	setWidget(widget);

	QGridLayout *layout = new QGridLayout(widget);
	layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
	layout->setColumnStretch(0, 1);
	int row = 0;

	QLabel *module_label = new QLabel(tr("Alarm module"));
	module = new QComboBox(this);
	module->addItems(ModuleList::availableModules());
	int current_idx = module->findText(settings.value("module").toString(), Qt::MatchFixedString); //case insensitive search
	if(current_idx == -1) //invalid module, use default
		current_idx = module->findText("Movement");
	module->setCurrentIndex(current_idx);
	layout->addWidget(module_label, row, 0);
	layout->addWidget(module, row, 1);

	row++;
	module_settings_layout = new QStackedLayout();
	layout->addLayout(module_settings_layout, row, 0, 1, 2);
	foreach(QString modulename, ModuleList::availableModules()) {
		module_settings_layout->addWidget(ModuleList::getSettingsInstance(modulename, this));
	}
	connect(module, SIGNAL(activated(int)),
		module_settings_layout, SLOT(setCurrentIndex(int)));
	module_settings_layout->setCurrentIndex(current_idx);

	row++;
	sound_filename = new QMaemo5ValueButton("Sound File", this);
	sound_filename->setValueText(settings.value("sound_filename", SOUND_FILE).toString());
	volume = new QSlider(Qt::Horizontal, this);
	volume->setRange(10, 100);
	volume->setValue(settings.value("max_volume", MAX_VOLUME).toInt());
	volume->setMinimumWidth(100);
	layout->addWidget(sound_filename, row, 0);
	layout->addWidget(volume, row, 1);
	connect(sound_filename, SIGNAL(clicked()),
		this, SLOT(pickSoundFile()));

	row++;
	vibration = new QCheckBox(tr("Enable vibration"));
	vibration->setChecked(settings.value("use_vibration", USE_VIBRATION).toBool());
	layout->addWidget(vibration, row, 0, 1, 2);

	row++;
	QLabel *alarm_timeout_label = new QLabel(tr("Completely shut down after"));
	alarm_timeout = new QSpinBox();
	alarm_timeout->setSuffix(" min");
	alarm_timeout->setRange(1, 60);
	alarm_timeout->setValue(settings.value("alarm_timeout", ALARM_TIMEOUT).toInt());
	layout->addWidget(alarm_timeout_label, row, 0);
	layout->addWidget(alarm_timeout, row, 1);

	row++;
	QLabel *inactivity_timeout_label = new QLabel(tr("Restart alarm if phone not moved for"));
	inactivity_timeout = new QSpinBox();
	inactivity_timeout->setSuffix(" s");
	inactivity_timeout->setValue(settings.value("inactivity_timeout", INACTIVITY_TIMEOUT).toInt());
	layout->addWidget(inactivity_timeout_label, row, 0);
	layout->addWidget(inactivity_timeout, row, 1);

	row++;
	QLabel *snooze_time_label = new QLabel(tr("Snooze time"));
	snooze_time = new QSpinBox();
	snooze_time->setSuffix(" min");
	snooze_time->setRange(1, 60);
	snooze_time->setValue(settings.value("snooze_time", SNOOZE_TIME).toInt());
	layout->addWidget(snooze_time_label, row, 0);
	layout->addWidget(snooze_time, row, 1);

	row++;
	QLabel *num_snooze_max_label = new QLabel(tr("Max. number of snoozes (0 to disable snooze)"));
	num_snooze_max = new QSpinBox();
	num_snooze_max->setValue(settings.value("num_snooze_max", NUM_SNOOZE_MAX).toInt());
	layout->addWidget(num_snooze_max_label, row, 0);
	layout->addWidget(num_snooze_max, row, 1);

	row++;
	fullscreen = new QCheckBox(tr("Disable multi-tasking (including Phone app)"));
	fullscreen->setChecked(settings.value("fullscreen", FULLSCREEN).toBool());
	layout->addWidget(fullscreen, row, 0, 1, 2);

	widget->setLayout(layout);
}

void Settings::closeEvent(QCloseEvent*)
{
	save();
	delete module_settings; //save module settings
	deleteLater();
}

void Settings::pickSoundFile()
{
	QString name = QFileDialog::getOpenFileName(this, tr("Choose Sound File"), sound_filename->valueText(), "Sound Files(*.wav *.mp3 *.ogg *.aac)");
	if(!name.isEmpty())
		sound_filename->setValueText(name);
}

void Settings::save()
{
	settings.setValue("module", module->currentText());
	settings.setValue("sound_filename", sound_filename->valueText());
	settings.setValue("max_volume", volume->value());
	settings.setValue("use_vibration", vibration->isChecked());
	settings.setValue("alarm_timeout", alarm_timeout->value());
	settings.setValue("inactivity_timeout", inactivity_timeout->value());
	settings.setValue("snooze_time", snooze_time->value());
	settings.setValue("num_snooze_max", num_snooze_max->value());
	settings.setValue("fullscreen", fullscreen->isChecked());
	settings.sync();
}
