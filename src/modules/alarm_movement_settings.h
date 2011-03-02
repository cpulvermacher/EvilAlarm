#ifndef ALARM_MOVEMENT_SETTINGS_H
#define ALARM_MOVEMENT_SETTINGS_H

#include <QtGui>

#include "module_settings.h"

class Accelerometer;

class AlarmMovementSettings : public ModuleSettings {
	Q_OBJECT
public:
	AlarmMovementSettings(QWidget *parent);
	~AlarmMovementSettings();
	void save();
private slots:
	void accelUpdate(int, int, int);
private:
	QSpinBox *threshold;
	QLabel *threshold_label;
	Accelerometer *accel;
};

#endif
