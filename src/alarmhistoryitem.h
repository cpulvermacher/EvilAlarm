#ifndef alarmhistoryitem_H
#define alarmhistoryitem_H

#include <QtGui>

const int NUM_HISTORY_ITEMS = 6;

class AlarmHistoryItem : public QPushButton
{
    Q_OBJECT
public:
    AlarmHistoryItem(QTime t, int used, QWidget *parent);
    ~AlarmHistoryItem();

    void setNumUsed(int n) { num_used = n; }
    int numUsed() const { return num_used; }
    void setTime(QTime t) { alarm_time = t; }
    QTime time() const { return alarm_time; }

public slots:
    void longPress();
    void emitRemove();
    void emitSelected();

signals:
    void remove(QTime);
    void selected(QTime);

private:
    QTimer button_held_timer;
    QTime alarm_time;
    int num_used;
};
#endif
