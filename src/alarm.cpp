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
#include "preferences.h"

#include <QtDBus>
#include <QtGui>
#include <QX11Info>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <iostream>


Alarm::Alarm(QWidget *parent, bool testing):
	QDialog(parent),
	label(new QLabel(this)),
	noise(new Phonon::MediaObject(this)),
	audio_output(new Phonon::AudioOutput(Phonon::MusicCategory,  this)),
	testing(testing)
{
	std::cout << "init Alarm\n";
	setWindowTitle("EvilAlarm");
	if(!testing)
		setWindowState(windowState() | Qt::WindowFullScreen);

	Phonon::createPath(noise, audio_output);

	//load settings
	if(!testing) {
		QSettings settings;
		noise->setCurrentSource(Phonon::MediaSource(settings.value("sound_filename", "/home/user/MyDocs/.sounds/Ringtones/Beep.aac").toString()));
		max_volume = settings.value("sound_volume", 1.0).toFloat();
		alarm_timeout = settings.value("alarm_timeout", ALARM_TIMEOUT).toInt();
		inactivity_timeout = settings.value("inactivity_timeout", INACTIVITY_TIMEOUT).toInt();
	}

	grabZoomKeys(true);

	//setup ui
	QVBoxLayout* layout1 = new QVBoxLayout();
	QHBoxLayout* layout2 = new QHBoxLayout();
	QLabel *text_label = new QLabel(testing?"<h2>Just testing</h2>":"<h2>Wake up!</h2>", this);
	layout1->addWidget(text_label);
	QLabel *icon_label = new QLabel(this);
	icon_label->setPixmap(QPixmap("/usr/share/icons/hicolor/64x64/apps/evilalarm.png"));
	layout2->addWidget(icon_label);
	layout2->addWidget(label);
	layout1->addLayout(layout2);
	setLayout(layout1);

	if(!testing)
		start();
	std::cout << "finished\n";
}

//actually starts the alarm, needs to be called manually when testing
void Alarm::start()
{
	std::cout << "start\n";

	connect(noise, SIGNAL(aboutToFinish()),
		this, SLOT(repeatSound()));

	//switch to 'general' profile
	QDBusInterface interface("com.nokia.profiled", "/com/nokia/profiled", "com.nokia.profiled");
	QDBusReply<QString> profile = interface.call("get_profile");
	old_profile = profile;
	interface.call("set_profile", "general");

	noise->play();
	alarm_started = QTime::currentTime();

	accel = new Accelerometer(this, ACCELEROMETER_POLL_MSEC);
	connect(accel, SIGNAL(orientationChanged(int, int, int)),
		this, SLOT(accelUpdate(int, int, int)));
}

Alarm::~Alarm()
{
	std::cout << "~Alarm\n";
	delete accel;
	delete noise;
	delete audio_output;
}


void Alarm::accelUpdate(int x, int y, int z)
{
	std::cout << ".";
	static int lastx = x;
	static int lasty = y;
	static int lastz = z;
	static QTime last_active = QTime::currentTime();

	if(alarm_started.elapsed()/1000 > alarm_timeout*60) {
		close();
	}

	const qreal volume_step = 0.1;
	if(qAbs(lastx - x) > ACCELEROMETER_THRESHOLD
	or qAbs(lasty - y) > ACCELEROMETER_THRESHOLD
	or qAbs(lastz - z) > ACCELEROMETER_THRESHOLD) { //device moved
		audio_output->setVolume(audio_output->volume() - volume_step);
		last_active.restart();
	} else if(last_active.elapsed()/1000 > inactivity_timeout) { //not moved for a while
		noise->play();
		if(audio_output->volume() < max_volume)
			audio_output->setVolume(audio_output->volume() + volume_step);
	}

	//Phonon thinks it's OK to set the volume to 'nan' if we set something < 0
	if(audio_output->volume() != audio_output->volume()) {
		audio_output->setVolume(0.0);
		noise->pause();
	}

	int secs_remaining = alarm_timeout*60 - alarm_started.elapsed()/1000;
	if(secs_remaining < 60) {
		label->setText(tr("%1 seconds remaining").arg(secs_remaining));
	} else {
		int mins_remaining = qRound(secs_remaining/60);
		label->setText(tr("%1 minutes remaining").arg(mins_remaining));
	}

	lastx = x; lasty = y; lastz = z;
}

void Alarm::closeEvent(QCloseEvent*)
{
	std::cout << "closeEvent()";
	hide();
	noise->stop();
	noise->deleteLater();

	//reset profile
	QDBusInterface interface("com.nokia.profiled", "/com/nokia/profiled", "com.nokia.profiled");
	interface.call("set_profile", old_profile);

	grabZoomKeys(false);
}


void Alarm::repeatSound()
{
	std::cout << "repeatSound()";
	noise->enqueue(noise->currentSource());
}


void Alarm::grabZoomKeys(bool grab)
{
	unsigned long val = (grab)?1:0;
	Atom atom = XInternAtom(QX11Info::display(), "_HILDON_ZOOM_KEY_ATOM", False);
	if(!atom) {
		qWarning("Couldn't get zoom key atom");
		return;
	}
	XChangeProperty(QX11Info::display(), winId(), atom, XA_INTEGER,
		32, PropModeReplace, reinterpret_cast<unsigned char *>(&val), 1);
}

void Alarm::test(QWidget *parent, QString sound_filename, float max_vol, int al_timeout, int in_timeout)
{
	std::cout << "test\n";
	Alarm *alarm = new Alarm(parent, true);
	alarm->max_volume = max_vol;
	//alarm->alarm_timeout = al_timeout; //TODO put this in
	alarm->alarm_timeout = 1;
	alarm->inactivity_timeout = in_timeout;
	alarm->noise->setCurrentSource(Phonon::MediaSource(sound_filename));

	alarm->start();
	alarm->exec();

	delete alarm;
	std::cout << "test finished\n";
}
