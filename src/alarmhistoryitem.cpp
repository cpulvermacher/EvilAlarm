
#include "alarmhistoryitem.h"


AlarmHistoryItem::AlarmHistoryItem(QTime t, int used, QWidget *parent):
	QPushButton(parent),
	alarm_time(t),
	num_used(used),
    total(used)
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

	connect(this, SIGNAL(clicked()),
		this, SLOT(emitSelected()));
}

AlarmHistoryItem::~AlarmHistoryItem()
{
}

void AlarmHistoryItem::updateItem()
{
	//assuming n items, if used equally they should have 15pt
	const double font_size_pt = qBound(12.0, 15.0*NUM_HISTORY_ITEMS*double(num_used)/total, 30.0);

	setText(alarm_time.toString(Qt::SystemLocaleShortDate));
	setStyleSheet(tr("QPushButton { font-size: %1pt; }").arg(font_size_pt, 0, 'g', 3));
}

void AlarmHistoryItem::emitSelected()
{
	emit selected(alarm_time);
}
