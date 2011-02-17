#ifndef ALARM_MOVEMENT_SETTINGS_H
#define ALARM_MOVEMENT_SETTINGS_H

#include <QtGui>

class Accelerometer;

class AlarmMovementSettings : public QWidget {
	Q_OBJECT
public:
	AlarmMovementSettings(QWidget *parent);
	~AlarmMovementSettings();
private slots:
	void accelUpdate(int, int, int);
private:
	QSpinBox *threshold;
	QLabel *threshold_label;
	Accelerometer *accel;
};

#endif
