#include "accelerometer.h"
#include "alarm_movement_settings.h"
#include "settings.h"

#include <QtGui>

AlarmMovementSettings::AlarmMovementSettings(QWidget *parent):
	ModuleSettings(parent)
{
	accel = new Accelerometer(this, ACCELEROMETER_POLL_MSEC);
	connect(accel, SIGNAL(orientationChanged(int, int, int)),
		this, SLOT(accelUpdate(int, int, int)));

	QGridLayout *layout = new QGridLayout(this);
	layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
	layout->setColumnStretch(0, 1);

	QSettings settings;
	threshold_label = new QLabel(tr("Accelerometer threshold"));
	threshold = new QSpinBox();
	threshold->setRange(1, 3000);
	threshold->setValue(settings.value("movement_threshold", ACCELEROMETER_THRESHOLD).toInt());
	layout->addWidget(threshold_label, 0, 0);
	layout->addWidget(threshold, 0, 1);
}

AlarmMovementSettings::~AlarmMovementSettings()
{
	delete accel;
}

void AlarmMovementSettings::accelUpdate(int x, int y, int z)
{
	static int lastx = 0;
	static int lasty = 0;
	static int lastz = 0;

	int max_diff = qMax(qAbs(lastx - x), qMax(qAbs(lasty - y), qAbs(lastz - z)));
	bool over_threshold = max_diff>threshold->value();

	QString text = QString("Accelerometer threshold (");
	if(over_threshold)
		text += "<font color='#ff8000'>";

	text += tr("current value: %1").arg(max_diff);

	if(over_threshold)
		text += "</font>";
	text += ")";
	
	threshold_label->setText(text);
	lastx = x; lasty = y; lastz = z;
}

void AlarmMovementSettings::save()
{
	QSettings settings;
	settings.setValue("movement_threshold", threshold->value());
	settings.sync();
}

