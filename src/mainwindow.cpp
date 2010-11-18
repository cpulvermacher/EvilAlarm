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
#include "daemon.h"
#include "mainwindow.h"
#include "preferences.h"

#include <QtMaemo5>
#include <QtGui>


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	QSettings settings;
	setWindowTitle("EvilAlarm");

	//create menu
	menuBar()->addAction(tr("&Preferences"), this, SLOT(showPreferences()));
	menuBar()->addAction(tr("&About"), this, SLOT(about()));

	QWidget *centerwidget = new QWidget(this);
	QHBoxLayout *layout1 = new QHBoxLayout(centerwidget);

	activate_alarm = new QCheckBox(this);
	activate_alarm->setMaximumWidth(70);

	time_button = new QMaemo5ValueButton(tr("Wake me at ..."), this);
	time_picker = new QMaemo5TimePickSelector(this);
	//time_picker->setMinuteStep(5);
	time_picker->setCurrentTime(settings.value("wake_at", QTime::currentTime()).toTime());
	time_button->setPickSelector(time_picker);

	layout1->addWidget(activate_alarm);
	layout1->addWidget(time_button);

	setCentralWidget(centerwidget);

	connect(activate_alarm, SIGNAL(toggled(bool)),
		this, SLOT(toggleAlarm()));
	connect(&timer, SIGNAL(timeout()),
		activate_alarm, SLOT(toggle()));

	if(Daemon::isRunning())
		activate_alarm->toggle();
}


void MainWindow::showPreferences()
{
	Preferences *p = new Preferences(this);
	p->exec();
	delete p;
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

	} else {
		timer.stop();
		Daemon::stop();
	}

	time_button->setEnabled(!activate_alarm->isChecked());
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("About EvilAlarm"),
		tr("<center><h1>EvilAlarm 0.3</h1>\
An alarm clock which cannot be turned off while asleep\
<small><p>&copy;2010 Christian Pulvermacher &lt;pulvermacher@gmx.de&gt</p></small></center>\
<p>This program is free software; License: <a href=\"http://www.gnu.org/licenses/gpl-2.0.html\">GNU GPL 2</a> or later.</p>"));
}
