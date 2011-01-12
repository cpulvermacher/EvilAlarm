#include "alarm.h"
#include "daemon.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	QCoreApplication::setOrganizationName("EvilAlarm");
	QCoreApplication::setApplicationName("EvilAlarm");

	//ensure destructors are called
	QScopedPointer<QWidget> widget_pointer;

	if(QCoreApplication::arguments().contains(QString("--daemon"))) {
		new Daemon;
	} else if(QCoreApplication::arguments().contains(QString("--wakeup"))) {
		widget_pointer.reset(Alarm::getModuleInstance());
		widget_pointer->show();
	} else {
		widget_pointer.reset(new MainWindow);
		widget_pointer->show();
	}
	return app.exec();
}
