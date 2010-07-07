#include "accelerometer.h"

#include <QtDBus>
#include <QTimer>


#include <mce/mode-names.h>
#include <mce/dbus-names.h>

Accelerometer::Accelerometer(QObject *parent, int interval):
	QObject(parent),
	timer(new QTimer(this)),
	interval(interval)
{
	connect(timer, SIGNAL(timeout()),
		this, SLOT(tick()));
	timer->setInterval(interval);
	timer->start();
	/*
	QDBusConnection::systemBus().call(QDBusMessage::createMethodCall(MCE_SERVICE, MCE_REQUEST_PATH, MCE_REQUEST_IF, MCE_DEVICE_ORIENTATION_GET));
	QDBusConnection::systemBus().connect("", MCE_SIGNAL_PATH, MCE_SIGNAL_IF, QString(), this, SLOT(orientationUpdate(QString, QString, QString, int, int, int)));
	*/
}


Accelerometer::~Accelerometer()
{
//	 QDBusConnection::systemBus().disconnect("", MCE_SIGNAL_PATH, MCE_SIGNAL_IF, MCE_DEVICE_ORIENTATION_SIG, this, SLOT(orientationUpdate(QString,QString,QString, int, int, int)));
	QDBusConnection::systemBus().call(QDBusMessage::createMethodCall(MCE_SERVICE, MCE_REQUEST_PATH, MCE_REQUEST_IF, MCE_ACCELEROMETER_DISABLE_REQ));
}


void Accelerometer::tick()
{
	QDBusConnection::systemBus().callWithCallback(
		QDBusMessage::createMethodCall(MCE_SERVICE, MCE_REQUEST_PATH, MCE_REQUEST_IF, MCE_DEVICE_ORIENTATION_GET),
		this, SLOT(orientationUpdate(QString, QString, QString, int, int, int))
	);
}

void Accelerometer::orientationUpdate(const QString &/*rotation*/, const QString &/*stand*/, const QString &/*face*/, int x, int y, int z) {
	emit orientationChanged(x, y, z);
}
