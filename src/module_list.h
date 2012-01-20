#ifndef MODULE_LIST_H
#define MODULE_LIST_H

#include <QtGui>

class Alarm;
class ModuleSettings;

namespace ModuleList
{
//get the user-configured module
Alarm* getModuleInstance(QWidget *parent = 0);

//get a settings widget for the given module name
ModuleSettings* getSettingsInstance(QString name, QWidget *parent = 0);

QStringList availableModules();
};

#endif
