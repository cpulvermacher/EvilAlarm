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

#include <QtDBus>

#include <iostream>

//same as PatternIncomingCall defaults
const int VIBRATOR_ON_MSECS = 900;
const int VIBRATOR_OFF_MSECS = 1000;

Backend::Backend():
	noise(new Phonon::MediaObject(this)),
	audio_output(new Phonon::AudioOutput(Phonon::MusicCategory,  this)),
	alarm_playing(false),
	is_vibrating(false)
{
	QSettings settings;

	Phonon::createPath(noise, audio_output);
	noise->setCurrentSource(Phonon::MediaSource(settings.value("sound_filename", SOUND_FILE).toString()));
	max_volume = settings.value("max_volume", MAX_VOLUME).toInt();
	volume = max_volume;
	use_vibration = settings.value("use_vibration", USE_VIBRATION).toBool();

	//TODO: if sound_filename isn't found, enable vibration anyway

	connect(noise, SIGNAL(aboutToFinish()),
		this, SLOT(repeatSound()));
	audio_output->setVolume(100);

	//save old profile
	QDBusInterface interface("com.nokia.profiled", "/com/nokia/profiled", "com.nokia.profiled");
	QDBusReply<QString> profile = interface.call("get_profile");
	old_profile = profile;

	//save old volume
	QProcess pasr;
	pasr.setStandardOutputFile("/tmp/evilalarm_sinkstate.backup");
	pasr.start("pasr --store");
	pasr.waitForFinished(2000); //don't destroy process before it's done

	std::cout << "Backend() finished\n";
}

Backend::~Backend()
{
	std::cout << "~Backend()\n";

	//restore
	QProcess pasr;
	pasr.setStandardInputFile("/tmp/evilalarm_sinkstate.backup");
	pasr.start("pasr --restore");
	pasr.waitForFinished(2000); //don't destroy process before it's done
	QProcess::execute("rm /tmp/evilalarm_sinkstate.backup");

	//reset profile
	QDBusInterface interface("com.nokia.profiled", "/com/nokia/profiled", "com.nokia.profiled");
	interface.call("set_profile", old_profile);

	delete noise;
	delete audio_output;
}

void Backend::play()
{
	if(alarm_playing)
		return;

	//switch to 'general' profile (user can switch it back, so we do this here to foil that)
	QDBusInterface interface("com.nokia.profiled", "/com/nokia/profiled", "com.nokia.profiled");
	interface.call("set_profile", "general");

	//and make sure volume is ok
	setVolume(volume);

	alarm_playing = true;
	noise->play();
	if(use_vibration)
		startVibrator();

}

void Backend::pause()
{
	if(!alarm_playing)
		return;

	alarm_playing = false;
	noise->pause();
	stopVibrator();
}

//increase/decrease volume by 1/7 of max_volume, or at least 1%
void Backend::volumeUp() { setVolume(volume + qMax(qRound(max_volume/7.0), 1)); }
void Backend::volumeDown() { setVolume(volume - qMax(qRound(max_volume/7.0), 1)); }

void Backend::setVolume(int v)
{
	std::cout << "setVolume(" << v << ")\n";
	if(v <= 0) {
		std::cout << "volume to 0, pause playback\n";
		volume = 0;
		pause();
	} else if(v > max_volume) {
		volume = max_volume;
	} else {
		if(volume == 0 and v > volume) {//volume up from 0?
			std::cout << "volume up from 0\n";
			play();
		}
		volume = v;
	}

	QProcess::execute(QString("pasr -u -s sink-input-by-media-role:x-maemo -l %1").arg(volume));
}

void Backend::repeatSound()
{
	noise->enqueue(noise->currentSource());
}

bool Backend::isPlaying() { return alarm_playing; }

bool Backend::isVibrating() { return is_vibrating; }

void Backend::setVibratorStateOff() { 
	std::cout << "setVibratorStateOff()\n";
	is_vibrating = false; }

void Backend::startVibrator()
{
	std::cout << "startVibrator()\n";
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
