#include "alarm.h"
#include "module_list.h"
#include "module_settings.h"
#include "modules/alarm_blubbels.h"
#include "modules/alarm_blubbels_settings.h"
#include "modules/alarm_normal.h"
#include "modules/alarm_movement.h"
#include "modules/alarm_movement_settings.h"


Alarm* ModuleList::getModuleInstance(bool test, QWidget *parent)
{
    QSettings settings;
    const QString modulename = settings.value("module").toString().toLower();

    if(modulename == "normal")
        return new AlarmNormal(parent, test);
    else if(modulename == "movement")
        return new AlarmMovement(parent, test);
    else if(modulename == "blubbels")
        return new AlarmBlubbels(parent, test);
    else
        return new AlarmNormal(parent, test);
}

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
    return QStringList() << "Normal" << "Blubbels" << "Movement";
}
