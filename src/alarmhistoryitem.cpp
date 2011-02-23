
#include "alarmhistoryitem.h"

int AlarmHistoryItem::total = 1;

AlarmHistoryItem::AlarmHistoryItem(QTime t, int used, QWidget *parent):
	QPushButton(parent),
	alarm_time(t),
	num_used(used)
{
	total += num_used;

	connect(this, SIGNAL(clicked()),
		this, SLOT(emitSelected()));
}

AlarmHistoryItem::~AlarmHistoryItem()
{
	total -= num_used;
}

void AlarmHistoryItem::updateItem()
{
	double font_size_pt = qMax(50*double(num_used)/total, 12.0);
	setText(alarm_time.toString(Qt::SystemLocaleShortDate) + QString(" %1pt").arg(font_size_pt, 0, 'g', 3));
	setStyleSheet(tr("QPushButton { font-size: %1pt; }").arg(font_size_pt, 0, 'g', 3));
}

void AlarmHistoryItem::emitSelected()
{
	emit selected(alarm_time);
}
