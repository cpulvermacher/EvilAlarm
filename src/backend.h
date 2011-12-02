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
#ifndef BACKEND_H
#define BACKEND_H

#include <QtCore>
#include <Phonon>

const char* KEEPVOLUME_PATH = "/opt/evilalarm/share/keepvolume.sh";

//handles audio & vibration
class Backend : public QObject {
    Q_OBJECT
public:
    Backend(QObject *parent);
    ~Backend();
    bool isPlaying(); //audio is playing
    bool isVibrating(); //is vibrator on right _now_ ?
public slots:
    void play();
    void pause();
    void volumeUp();
    void volumeDown();
    void setVolume(qreal v);

private slots:
    void checkForHang();
    void handleAudioStateChange(Phonon::State newstate);
    void repeatSound();
    void setVibratorStateOff();
    void startVibrator();
    void stopVibrator();
private:
    Phonon::MediaObject* noise;
    Phonon::AudioOutput* audio_output;
    QTime alarm_started;
    bool alarm_playing;
    bool is_vibrating;
    bool use_vibration;
    bool snoozing;
    int alarm_timeout;
    qreal volume;
    QProcess keepvolume;
    QString old_profile;
};
#endif
