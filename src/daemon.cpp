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
	const QTime wake_at = settings.value("wake_at").toTime();
	if(!wake_at.isValid()) {
		std::cerr << "Invalid alarm time, aborting!\n";
		exit(1);
	}

	int msecs = QTime::currentTime().msecsTo(wake_at);
	if(msecs < 0) //alarm tomorrow?
		msecs += 24*60*60*1000; //+24h
	
	QTimer::singleShot(msecs, this, SLOT(wake()));

	std::cout << "Daemon started\n";
}


void Daemon::wake()
{
	//start evilalarm --wakeup
	ui_process = new QProcess(this);
	connect(ui_process, SIGNAL(finished(int, QProcess::ExitStatus)),
		this, SLOT(uiFinished(int, QProcess::ExitStatus)));
	ui_process->start(QCoreApplication::applicationFilePath(), QStringList("--wakeup"));
	std::cout << "evilalarm --wakeup started\n";
}


void Daemon::uiFinished(int, QProcess::ExitStatus status)
{
	delete ui_process;

	if(status == QProcess::CrashExit) {
		std::cerr << "UI probably got killed, restarting\n";
		wake();
	} else {
		//UI exited normally, stop daemon
		QSettings settings;
		settings.setValue("daemon_pid", 0);
		settings.sync();

		std::cout << "Alarm finished, daemon shutting down\n";
		QCoreApplication::quit();
	}
}


//static members
void Daemon::start()
{
	if(isRunning())
		return;

	qint64 pid;
	const bool success = QProcess::startDetached(QCoreApplication::applicationFilePath(), QStringList("--daemon"), QString(), &pid);
	if(!success) {
		std::cerr << "Couldn't start evilalarm daemon!\n";
		exit(1);
	}

	QSettings settings;
	settings.setValue("daemon_pid", pid);
	settings.sync();
}

void Daemon::stop()
{
	if(!isRunning())
		return;

	std::cout << "Stopping daemon\n";	
	
	QSettings settings;
	const int pid = settings.value("daemon_pid", 0).toInt();

	QProcess::execute(QString("kill %1").arg(pid));

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
	const bool running = QFile::exists(QString("/proc/%1/").arg(pid));
	if(!running) {
		std::cerr << "Daemon didn't shut down cleanly last time, removing old PID\n";

		settings.setValue("daemon_pid", 0);
		settings.sync();
	}

	return running;
}
