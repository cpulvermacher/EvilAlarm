#include "alarm.h"
#include "daemon.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	QCoreApplication::setOrganizationName("EvilAlarm");
	QCoreApplication::setApplicationName("EvilAlarm");

	if(QCoreApplication::arguments().contains(QString("--daemon"))) {
		new Daemon;
	} else if(QCoreApplication::arguments().contains(QString("--wakeup"))) {
		QWidget *widget = new Alarm;
		widget->show();
	} else {
		QWidget *widget = new MainWindow;
		widget->show();
	}
	return app.exec();
}
