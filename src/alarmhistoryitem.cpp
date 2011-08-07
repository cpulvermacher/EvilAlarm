
#include "alarmhistoryitem.h"

int AlarmHistoryItem::total = 1;

AlarmHistoryItem::AlarmHistoryItem(QTime t, int used, QWidget *parent):
	QPushButton(parent),
	alarm_time(t),
	num_used(used)
{
	total += num_used;
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

	connect(this, SIGNAL(clicked()),
		this, SLOT(emitSelected()));
}

AlarmHistoryItem::~AlarmHistoryItem()
{
	total -= num_used;
}

void AlarmHistoryItem::updateItem()
{
	//assuming n items, if used equally they should have 15pt
	const int num_items = 6;
	const double font_size_pt = qBound(12.0, 15.0*num_items*double(num_used)/total, 30.0);

	setText(alarm_time.toString(Qt::SystemLocaleShortDate));
	setStyleSheet(tr("QPushButton { font-size: %1pt; }").arg(font_size_pt, 0, 'g', 3));
}

void AlarmHistoryItem::emitSelected()
{
	emit selected(alarm_time);
}
