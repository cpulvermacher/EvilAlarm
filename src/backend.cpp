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

#include <iostream>


//same as PatternIncomingCall defaults
const int VIBRATOR_ON_MSECS = 900;
const int VIBRATOR_OFF_MSECS = 1000;

const qreal VOLUME_STEP = 0.1;


Backend::Backend(QObject *parent):
    QObject(parent),
    noise(new Phonon::MediaObject(this)),
    audio_output(new Phonon::AudioOutput(Phonon::MusicCategory,  this)),
    alarm_playing(false),
    is_vibrating(false),
    volume(1.0)
{
    QSettings settings;

    max_volume = 0.01 * settings.value("max_volume", MAX_VOLUME).toInt(); //setting is in percent
    use_vibration = settings.value("use_vibration", USE_VIBRATION).toBool();
    QString sound_filename = settings.value("sound_filename", SOUND_FILE).toString();

    connect(noise, SIGNAL(stateChanged(Phonon::State, Phonon::State)),
            this, SLOT(handleAudioStateChange(Phonon::State)));
    connect(noise, SIGNAL(aboutToFinish()),
            this, SLOT(repeatSound()));

    Phonon::createPath(noise, audio_output);
    noise->setCurrentSource(Phonon::MediaSource(sound_filename));

    QTimer *hangcheck_timer = new QTimer(this);
    hangcheck_timer->setInterval(500);
    connect(hangcheck_timer, SIGNAL(timeout()),
            this, SLOT(checkForHang()));
    hangcheck_timer->start();
}

Backend::~Backend()
{
    std::cout << "~Backend\n";
    pause();

    delete audio_output;
    delete noise;
}

void Backend::play()
{
    std::cout << "Backend::play()\n";
    alarm_playing = true;

    noise->play();
    if(use_vibration)
        startVibrator();
}

void Backend::pause()
{
    std::cout << "Backend::pause()\n";
    alarm_playing = false;

    noise->pause();
    if(use_vibration)
        stopVibrator();
}

void Backend::volumeUp() { setVolume(volume + VOLUME_STEP); }
void Backend::volumeDown() { setVolume(volume - VOLUME_STEP); }

void Backend::setVolume(qreal v)
{
    if(v <= 0) {
        v = 0;
        pause();
    } else if(v > volume and !alarm_playing) {
        play();
    }

    if(v > 1.0)
        v = 1.0;

    volume = v;
    audio_output->setVolume(volume*max_volume);
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
    if(!is_vibrating)
        return;

    QDBusInterface interface("com.nokia.mce", "/com/nokia/mce/request", "com.nokia.mce.request", QDBusConnection::systemBus());
    interface.call("req_vibrator_pattern_deactivate", "PatternIncomingCall");

    QTimer::singleShot(VIBRATOR_OFF_MSECS, this, SLOT(startVibrator()));

    //phone doesn't stop vibrating right away...
    QTimer::singleShot(200, this, SLOT(setVibratorStateOff()));
}

void Backend::handleAudioStateChange(Phonon::State newstate)
{
    if(newstate == Phonon::ErrorState) {
        std::cout << "Error playing the audio file!\n";
        if(!use_vibration) {
            std::cout << "Enabling vibration as fallback.\n";

            use_vibration = true;
            if(alarm_playing)
                startVibrator();
        }
    }
}

//workaround for Qt bug occuring when pause() is called shortly before the audio file is over, resulting in no sound but a Phonon::PlayingState
void Backend::checkForHang()
{
    //only check if Phonon says it is playing (and we're supposed to play sound)
    if(!alarm_playing
            or noise->state() != Phonon::PlayingState
            or noise->totalTime() == -1)
        return;

    static int last_pos = -1;
    const int new_pos = noise->currentTime();
    if(last_pos == new_pos) {
        std::cout << "Audio hang detected, resetting.\n";
        noise->clear();
        QSettings settings;
        QString sound_filename = settings.value("sound_filename", SOUND_FILE).toString();
        noise->setCurrentSource(Phonon::MediaSource(sound_filename));
        noise->play();
    }
    last_pos = new_pos;
}
