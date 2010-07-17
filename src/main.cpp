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
#include "main.h"
#include "preferences.h"

#include <QtMaemo5>
#include <QtGui>


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	QSettings settings;
	setWindowTitle("EvilAlarm");

	//create menu
	menuBar()->addAction("&About", this, SLOT(about()));

	connect(&timer, SIGNAL(timeout()),
		this, SLOT(wake()));
	
	QWidget *centerwidget = new QWidget(this);
	QVBoxLayout *layout1 = new QVBoxLayout(centerwidget);

	time_button = new QMaemo5ValueButton("Wake me at ...", this);
	time_picker = new QMaemo5TimePickSelector(this);
	//time_picker->setMinuteStep(5);
	time_picker->setCurrentTime(settings.value("wake_at", QTime::currentTime()).toTime());
	time_button->setPickSelector(time_picker);

	QHBoxLayout *layout2 = new QHBoxLayout();
	QPushButton *pref_button = new QPushButton("Preferences", this);
	activate_button = new QPushButton("Activate", this);

	layout2->addWidget(pref_button);
	layout2->addWidget(activate_button);
	layout1->addWidget(time_button);
	layout1->addLayout(layout2);

	setCentralWidget(centerwidget);

	connect(pref_button, SIGNAL(clicked()),
		this, SLOT(showPreferences()));
	connect(activate_button, SIGNAL(clicked()),
		this, SLOT(toggleTimer()));
}


void MainWindow::showPreferences()
{
	Preferences *p = new Preferences(this);
	p->exec();
	delete p;
}


void MainWindow::toggleTimer()
{
	QTime wake_at = time_picker->currentTime();

	if(timer.isActive()) {
		timer.stop();
		time_button->setEnabled(true);
		activate_button->setText("Activate");
	} else {
		//check wether sound file exists
		QSettings settings;
		QFileInfo soundfile(settings.value("sound_filename", SOUND_FILE).toString());
		if(!soundfile.exists() or !soundfile.isReadable()) {
			QMaemo5InformationBox::information(this, "Cannot read sound file, please check your preferences.");
			return;
		}

		int msecs = QTime::currentTime().msecsTo(wake_at);
		if(msecs < 0)
			msecs += 24*60*60*1000; //24h in msec
		
		timer.setInterval(msecs);
		timer.start();

		time_button->setEnabled(false);
		activate_button->setText("Deactivate");

		settings.setValue("wake_at", wake_at);
		settings.sync();
	}
}

void MainWindow::wake()
{
	timer.stop();
	time_button->setEnabled(true);
	activate_button->setText("Activate");

	Alarm alarm(this);
	alarm.exec();
}


void MainWindow::about()
{
	QMessageBox::about(this, tr("About EvilAlarm"),
		tr("<center><h1>EvilAlarm 0.2</h1>\
An alarm clock which cannot be turned off while asleep\
<small><p>&copy;2010 Christian Pulvermacher &lt;pulvermacher@gmx.de&gt</p></small></center>\
<p>This program is free software; License: <a href=\"http://www.gnu.org/licenses/gpl-2.0.html\">GNU GPL 2</a> or later.</p>"));
}


void MainWindow::closeEvent(QCloseEvent* ev)
{
	if(timer.isActive()) {
		QMaemo5InformationBox::information(this, "Sorry, EvilAlarm can't wake you if you close it.");
		ev->ignore();
	} 
}


int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	QCoreApplication::setOrganizationName("EvilAlarm");
	QCoreApplication::setApplicationName("EvilAlarm");

	MainWindow *mw = new MainWindow;
	mw->show();
	return app.exec();
}
