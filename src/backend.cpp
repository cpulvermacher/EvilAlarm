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
#include "backend.h"
#include "settings.h"

#include <QFile>
#include <QtDBus>

#include <cstdlib>
#include <iostream>

//same as PatternIncomingCall defaults
const int VIBRATOR_ON_MSECS = 900;
const int VIBRATOR_OFF_MSECS = 1000;

const qreal VOLUME_STEP = 0.1;

Backend::Backend():
	noise(new Phonon::MediaObject(this)),
	audio_output(new Phonon::AudioOutput(Phonon::MusicCategory,  this)),
	alarm_playing(false),
	is_vibrating(false),
	volume(1.0)
{
	QSettings settings;

	Phonon::createPath(noise, audio_output);
	int max_volume = settings.value("max_volume", MAX_VOLUME).toInt();
	use_vibration = settings.value("use_vibration", USE_VIBRATION).toBool();
	QString sound_filename = settings.value("sound_filename", SOUND_FILE).toString();

	if(QFile::exists(sound_filename)) {
		noise->setCurrentSource(Phonon::MediaSource(sound_filename));
	} else {
		//no sound file, enable vibration as fallback
		std::cout << "no sound file, enable vibration as fallback\n";
		use_vibration = true;
	}

	connect(noise, SIGNAL(aboutToFinish()),
		this, SLOT(repeatSound()));

	//save old profile
	QDBusInterface interface("com.nokia.profiled", "/com/nokia/profiled", "com.nokia.profiled");
	QDBusReply<QString> profile = interface.call("get_profile");
	old_profile = profile;

	//save old volume
	QProcess pasr;
	pasr.setStandardOutputFile("/tmp/evilalarm_sinkstate.backup");
	pasr.start("pasr --store");
	pasr.waitForFinished(2000); //don't destroy process before it's done

	//reset volume and profile regularly
	keepvolume.start(QString("sh /usr/share/evilalarm/keepvolume.sh %1").arg(max_volume));
}

Backend::~Backend()
{
	//TODO: hangs upon execution again :(
	pause();
	std::cout << "~Backend\n";

	keepvolume.kill();
	keepvolume.waitForFinished(2000); //don't destroy process before it's done

	std::cout << ".\n";

	//restore
	QProcess pasr;
	pasr.setStandardInputFile("/tmp/evilalarm_sinkstate.backup");
	pasr.start("pasr --restore");
	pasr.waitForFinished(2000); //don't destroy process before it's done

	std::cout << ".\n";
	//restore profile
	QDBusInterface interface("com.nokia.profiled", "/com/nokia/profiled", "com.nokia.profiled");
	interface.call("set_profile", old_profile);

	std::cout << ".\n";
	delete noise;
	delete audio_output;
	std::cout << "done\n";
}

void Backend::play()
{
	if(alarm_playing)
		return;

	std::cout << "Backend::play()\n";
	alarm_playing = true;

	noise->play();
	if(use_vibration)
		startVibrator();
}

void Backend::pause()
{
	if(!alarm_playing)
		return;

	std::cout << "Backend::pause()\n";
	alarm_playing = false;

	noise->pause();
	stopVibrator();
}

void Backend::volumeUp() { setVolume(volume + VOLUME_STEP); }
void Backend::volumeDown() { setVolume(volume - VOLUME_STEP); }

void Backend::setVolume(qreal v)
{
	if(v > 1.0)
		v = 1.0;

	if(v <= 0) {
		v = 0;
		pause();
	} else if(v > volume and !alarm_playing) {
		play();
	}
	volume = v;

	//doesn't work?
	audio_output->setVolume(volume);
}

void Backend::repeatSound() { noise->enqueue(noise->currentSource()); }

bool Backend::isPlaying() { return alarm_playing; }

bool Backend::isVibrating() { return is_vibrating; }

//used to change status after some delay, see stopVibrator()
void Backend::setVibratorStateOff() { is_vibrating = false; }

void Backend::startVibrator()
{
	if(!alarm_playing or is_vibrating)
		return;

	QDBusInterface interface("com.nokia.mce", "/com/nokia/mce/request", "com.nokia.mce.request", QDBusConnection::systemBus());
	interface.call("req_vibrator_pattern_activate", "PatternIncomingCall");
	is_vibrating = true;

	QTimer::singleShot(VIBRATOR_ON_MSECS, this, SLOT(stopVibrator()));
}

void Backend::stopVibrator()
{
	std::cout << "stopVibrator()\n";
	if(!is_vibrating)
		return;

	QDBusInterface interface("com.nokia.mce", "/com/nokia/mce/request", "com.nokia.mce.request", QDBusConnection::systemBus());
	interface.call("req_vibrator_pattern_deactivate", "PatternIncomingCall");

	QTimer::singleShot(VIBRATOR_OFF_MSECS, this, SLOT(startVibrator()));

	//phone doesn't stop vibrating right away...
	QTimer::singleShot(200, this, SLOT(setVibratorStateOff()));
}