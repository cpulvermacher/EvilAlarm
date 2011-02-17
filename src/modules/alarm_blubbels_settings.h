#ifndef ALARM_BLUBBELS_SETTINGS_H
#define ALARM_BLUBBELS_SETTINGS_H

#include <QtGui>

class AlarmBlubbelsSettings : public QWidget {
	Q_OBJECT
public:
	AlarmBlubbelsSettings(QWidget *parent);
	~AlarmBlubbelsSettings();
private:
	QSpinBox *threshold;
};

#endif
