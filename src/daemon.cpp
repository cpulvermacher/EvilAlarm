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

#include "daemon.h"

#include <QtCore>

#include <iostream>


Daemon::Daemon()
{
	QSettings settings;
	QTime wake_at = settings.value("wake_at").toTime();
	if(!wake_at.isValid()) {
		std::cerr << "Invalid alarm time, aborting\n";
		exit(1);
	}

	int msecs = QTime::currentTime().msecsTo(wake_at);
	if(msecs < 0) //alarm tomorrow?
		msecs += 24*60*60*1000; //+24h
	
	connect(&timer, SIGNAL(timeout()),
		this, SLOT(wake()));
	timer.setInterval(msecs);
	timer.start();
}


void Daemon::wake()
{
	timer.stop();

	//start evilalarm --wakeup
	ui_process = new QProcess(this);
	connect(ui_process, SIGNAL(finished(int, QProcess::ExitStatus)),
		this, SLOT(uiFinished()));
	ui_process->start(QCoreApplication::applicationFilePath(), QStringList("--wakeup"));
}


void Daemon::uiFinished()
{
	delete ui_process;

	//UI exited normally, stop daemon
	QSettings settings;
	settings.setValue("daemon_pid", 0);
	settings.sync();

	QCoreApplication::quit();
}


//static members
void Daemon::start()
{
	if(isRunning())
		return;

	qint64 pid;
	const bool success = QProcess::startDetached(QCoreApplication::applicationFilePath(), QStringList("--daemon"), QString(), &pid);
	Q_ASSERT(success);

	QSettings settings;
	settings.setValue("daemon_pid", pid);
	settings.sync();
}

void Daemon::stop()
{
	if(!isRunning())
		return;
	
	QSettings settings;
	const int pid = settings.value("daemon_pid", 0).toInt();

	const int status = QProcess::execute(QString("kill %1").arg(pid));
	Q_ASSERT(status == 0);

	settings.setValue("daemon_pid", 0);
	settings.sync();
}

bool Daemon::isRunning()
{
	QSettings settings;
	const int pid = settings.value("daemon_pid", 0).toInt();
	if(pid == 0)
		return false;

	//busybox doesn't support 'ps --pid' :(
	const int status = QProcess::execute(QString("cat /proc/%1/cmdline").arg(pid));
	if(status == 1) {
		//not running
		settings.setValue("daemon_pid", 0);
		settings.sync();

		return false;
	} else {
		return true;
	}
}
