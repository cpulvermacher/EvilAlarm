#ifndef ALARM_BLUBBELS_SETTINGS_H
#define ALARM_BLUBBELS_SETTINGS_H

#include <QtGui>

#include "module_settings.h"

class AlarmBlubbelsSettings : public ModuleSettings {
	Q_OBJECT
public:
	AlarmBlubbelsSettings(QWidget *parent);
	virtual ~AlarmBlubbelsSettings();
	virtual void save();
private:
	QSpinBox *threshold;
};

#endif
