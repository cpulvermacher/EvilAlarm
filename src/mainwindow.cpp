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
#include "alarm.h"
#include "alarmhistoryitem.h"
#include "daemon.h"
#include "mainwindow.h"
#include "module_list.h"
#include "settings.h"

#include <QtMaemo5>
#include <QtGui>

#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	setWindowTitle("EvilAlarm");
	setAttribute(Qt::WA_Maemo5StackedWindow);

	menuBar()->addAction(tr("Clear Alarm History"), this, SLOT(clearHistory()));
	menuBar()->addAction(tr("About"), this, SLOT(about()));

	QWidget *centerwidget = new QWidget(this);
	QHBoxLayout *layout1 = new QHBoxLayout();

	activate_alarm = new QCheckBox(this);
	activate_alarm->setMaximumWidth(70);

	QSettings settings;
	time_button = new QMaemo5ValueButton(tr("Wake me at ..."), this);
	time_picker = new QMaemo5TimePickSelector(this);
	time_picker->setCurrentTime(settings.value("wake_at", QTime::currentTime()).toTime());
	time_button->setPickSelector(time_picker);

	layout1->addWidget(activate_alarm);
	layout1->addWidget(time_button);

	//show history items
	history_scrollarea = new QScrollArea();
	QWidget *history_widget = new QWidget();
	history_layout = new QHBoxLayout();
	history_layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
	history_widget->setLayout(history_layout);
	history_widget->setMinimumHeight(240);
	history_scrollarea->setWidget(history_widget);
	reloadHistory();

	QHBoxLayout *layout2 = new QHBoxLayout();
	QPushButton *settings_button = new QPushButton(tr("Settings"), this);
	QPushButton *test_button = new QPushButton(tr("Test Alarm"), this);
	layout2->addWidget(settings_button);
	layout2->addWidget(test_button);

	QVBoxLayout *layout0 = new QVBoxLayout(centerwidget);
	layout0->addLayout(layout1);
	layout0->addWidget(history_scrollarea);
	layout0->addLayout(layout2);

	setCentralWidget(centerwidget);

	connect(activate_alarm, SIGNAL(toggled(bool)),
		this, SLOT(toggleAlarm()));
	connect(&timer, SIGNAL(timeout()),
		activate_alarm, SLOT(toggle()));
	connect(settings_button, SIGNAL(clicked()),
		this, SLOT(showSettings()));
	connect(test_button, SIGNAL(clicked()),
		this, SLOT(testAlarm()));

	if(Daemon::isRunning())
		activate_alarm->toggle();
	
	//check if we can save settings
	if(!settings.isWritable()) {
		QMaemo5InformationBox::information(this, tr("Can't write settings to /home/user/.config/EvilAlarm/EvilAlarm - Please fix its file permissions and restart EvilAlarm"));
		settings_button->setEnabled(false);
		activate_alarm->setEnabled(false);
		time_button->setEnabled(false);
	}
}


void MainWindow::about()
{
	QMessageBox::about(this, tr("About EvilAlarm"),
		tr("<center><h1>EvilAlarm 0.9</h1>\
An alarm clock which cannot be turned off while asleep\
<p><a href=\"http://garage.maemo.org/projects/evilalarm/\">garage.maemo.org/projects/evilalarm/</a></p>\
<small><p>&copy;2010, 2011 Christian Pulvermacher &lt;pulvermacher@gmx.de&gt;</p></small></center>\
<p>This program is free software; License: <a href=\"http://www.gnu.org/licenses/gpl-2.0.html\">GNU GPL 2</a> or later.</p>"));
}

void MainWindow::setAlarmTime(QTime time)
{
	if(activate_alarm->isChecked()) //unset old alarm
		activate_alarm->toggle();

	//set new one
	time_picker->setCurrentTime(time);
	activate_alarm->toggle();
}

void MainWindow::showSettings()
{
	static Settings *s = new Settings(this);
	s->show();
}

void MainWindow::testAlarm()
{
	Alarm* test_alarm = ModuleList::getModuleInstance(this);
	test_alarm->exec();
	delete test_alarm;

	//TODO: remove me
	//QTimer::singleShot(1000, this, SLOT(testAlarm()));
}

void MainWindow::toggleAlarm()
{
	if(activate_alarm->isChecked()) { 
		QSettings settings;

		QTime wake_at = time_picker->currentTime();
		settings.setValue("wake_at", wake_at);
		settings.sync();

		int msecs = QTime::currentTime().msecsTo(wake_at);
		if(msecs < 0) //alarm tomorrow?
			msecs += 24*60*60*1000; //+24h
		
		//toggle button again after alarm goes off
		timer.setInterval(msecs);
		timer.start();

		Daemon::start();

		const int minutes = (msecs/1000/60)%60;
		const int hours = (msecs/1000/60/60);
		QMaemo5InformationBox::information(this, tr("Time left until alarm: %1:%2")
			.arg(hours, 2, 10, QChar('0'))
			.arg(minutes, 2, 10, QChar('0')));

	} else {
		std::cout << "MainWindow::toggleAlarm() - stopping daemon\n";
		timer.stop();
		Daemon::stop();
		reloadHistory();
	}

	time_button->setEnabled(!activate_alarm->isChecked());
}


void MainWindow::reloadHistory()
{
	//remove any old items
	for(int i = 0; i < history.count(); i++) {
		AlarmHistoryItem* item = history[i];
		history_layout->removeWidget(item);
		delete item;
	}
	history.clear();

	//load history
	QSettings settings;
	settings.beginGroup("history");
	QStringList times = settings.childGroups();
	times.sort(); //times are in HH:MM:SS format, so they are sorted correctly
	foreach(QString time, times) {
		if(!settings.contains(time + "/used")) {
			continue; //just to be safe
		}

		int num_used = settings.value(time + "/used").toInt();
		history.append(new AlarmHistoryItem(QTime::fromString(time), num_used, this));
	}
	settings.endGroup();

	foreach(AlarmHistoryItem* item, history) {
		item->updateItem(); //make sure we're using the correct normalization

		history_layout->addWidget(item);
		connect(item, SIGNAL(selected(QTime)),
			this, SLOT(setAlarmTime(QTime)));
	}
}

void MainWindow::clearHistory()
{
	int ret = QMessageBox::question(this, tr("EvilAlarm"),
		tr("Do you really want to remove all history entries?"),
		QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
	if(ret == QMessageBox::Yes) {
		QSettings settings;
		settings.remove("history");
		settings.sync();

		reloadHistory();
	}
}
