
#include "alarmhistoryitem.h"

int AlarmHistoryItem::total = 1;

AlarmHistoryItem::AlarmHistoryItem(QTime t, int used, QWidget *parent):
	QPushButton(parent),
	alarm_time(t),
	num_used(used)
{
	total += num_used;
	updateItem();

	connect(this, SIGNAL(clicked()),
		this, SLOT(emitSelected()));
}

AlarmHistoryItem::~AlarmHistoryItem()
{
	total -= num_used;
}

void AlarmHistoryItem::updateItem()
{
	setText(alarm_time.toString(Qt::SystemLocaleShortDate));
	setStyleSheet(tr("QPushButton { font-size: %1pt; }").arg(40*double(num_used)/total, 0, 'g', 3));
}

void AlarmHistoryItem::emitSelected()
{
	emit selected(alarm_time);
}
