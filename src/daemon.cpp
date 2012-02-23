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


void Daemon::start()
{
    if(isRunning())
        return;

    QSettings settings;
    const QTime wake_at_time = settings.value("wake_at").toTime();
    if(!wake_at_time.isValid()) {
        std::cerr << "Invalid alarm time, aborting!\n";
        return;
    }

    QDateTime wake_at(QDate::currentDate(), wake_at_time);
    if(QDateTime::currentDateTime() > wake_at) //alarm tomorrow
        wake_at = wake_at.addDays(1);

    int seconds = QDateTime::currentDateTime().secsTo(wake_at);
    //when switching back from DST (i.e. an extra hour added) the same time occurs twice in 2 hours, with no way to distinguish them. To make things worse, the QML side thinks you mean the second hour, QDateTime takes the first.
    //We'll emulate the QML behaviour here so at least the '.. hours left' message is correct.
    const QDateTime wake_at_plus_1h(wake_at.date(), wake_at.time().addSecs(3600));
    if(wake_at.secsTo(wake_at_plus_1h) == 2*3600) {
        seconds += 60*60; //set alarm into extra hour
    }

    qint64 pid;
    const bool success = QProcess::startDetached(
            QString("evilalarm"),
            (QStringList() << "--daemon" << QString::number(seconds)), //args
            QString(), //working directory
            &pid);
    if(!success) {
        std::cerr << "Couldn't start evilalarm daemon!\n";
        return;
    }
    std::cout << "Daemon started, alarm at: " << qPrintable(wake_at.toString()) << "\n";

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

    //config thinks it's running, check if signals can be sent to process
    const bool running = (0 == QProcess::execute(QString("kill -0 %1").arg(pid)));
    if(!running) {
        //daemon gone, remove PID
        settings.setValue("daemon_pid", 0);
        settings.sync();
    }

    return running;
}
