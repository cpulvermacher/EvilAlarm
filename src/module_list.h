#ifndef MODULE_LIST_H
#define MODULE_LIST_H

#include <QtGui>

class Alarm;

namespace ModuleList {
	Alarm* getModuleInstance(QWidget *parent = 0);
	QWidget* getSettingsInstance(QString name, QWidget *parent = 0);
	QStringList availableModules();
};

#endif
