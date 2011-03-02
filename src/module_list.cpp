
#include "module_list.h"
#include "module_settings.h"
#include "modules/alarm_blubbels.h"
#include "modules/alarm_blubbels_settings.h"
#include "modules/alarm_movement.h"
#include "modules/alarm_movement_settings.h"


//get the user-configured module
Alarm* ModuleList::getModuleInstance(QWidget *parent)
{
	QSettings settings;
	QString modulename = settings.value("module").toString().toLower();

	if(modulename == "movement")
		return new AlarmMovement(parent);
	else if(modulename == "blubbels")
		return new AlarmBlubbels(parent);
	else
		return new AlarmMovement(parent);
}

//get a settings widget for the given module name
ModuleSettings* ModuleList::getSettingsInstance(QString name, QWidget *parent)
{
	name = name.toLower();
	if(name == "movement")
		return new AlarmMovementSettings(parent);
	else if(name == "blubbels")
		return new AlarmBlubbelsSettings(parent);
	else
		return 0;
}

QStringList ModuleList::availableModules()
{
	return QStringList() << "Blubbels" << "Movement";
}
