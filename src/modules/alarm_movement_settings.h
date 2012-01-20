#ifndef ALARM_MOVEMENT_SETTINGS_H
#define ALARM_MOVEMENT_SETTINGS_H

#include <QtGui>

#include "module_settings.h"

const int ACCELEROMETER_POLL_MSEC = 200;
const int ACCELEROMETER_THRESHOLD = 50;
const bool TURN_OFF_VIA_CHARGER = false;

class Accelerometer;

class AlarmMovementSettings : public ModuleSettings
{
    Q_OBJECT
public:
    explicit AlarmMovementSettings(QWidget *parent);
    virtual ~AlarmMovementSettings();
    virtual void save();
private slots:
    void accelUpdate(int, int, int);
private:
    QSpinBox *threshold;
    QLabel *threshold_label;
    QCheckBox *turn_off_via_charger;
    Accelerometer *accel;
};

#endif
