
#ifndef alarmhistoryitem_H
#define alarmhistoryitem_H

#include <QtGui>

class AlarmHistoryItem : public QPushButton {
	Q_OBJECT
public:
	AlarmHistoryItem(QTime t, int used, QWidget *parent);
	~AlarmHistoryItem();

	void setNumUsed(int n) { total -= num_used; num_used = n; total += n; }
	int numUsed() { return num_used; }
	void setTime(QTime t) { alarm_time = t; }
	QTime time() { return alarm_time; }
public slots:
	void updateItem();
	void emitSelected();

signals:
	void selected(QTime);

private:
	QTime alarm_time;
	int num_used;
	static int total;
};
#endif
