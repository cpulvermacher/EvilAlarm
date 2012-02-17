#include "alarm.h"
#include "module_list.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("EvilAlarm");
    QCoreApplication::setApplicationName("EvilAlarm");

    //ensure destructors are called
    QScopedPointer<QWidget> widget_pointer;

    if(QCoreApplication::arguments().contains(QString("--wakeup"))) {
        widget_pointer.reset(ModuleList::getModuleInstance());
    } else {
        widget_pointer.reset(new MainWindow);
    }
    widget_pointer->show();
    return app.exec();
}
