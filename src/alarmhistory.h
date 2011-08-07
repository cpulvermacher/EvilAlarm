#ifndef ALARMHISTORY_H
#define ALARMHISTORY_H

#include <QtGui>

class AlarmHistory : public QDialog
{
    Q_OBJECT
public:
    explicit AlarmHistory(QWidget *parent, int hours, int minutes);
    ~AlarmHistory();
signals:
    void setAlarm(int hours, int minutes);
private slots:
    void setAlarmTime(QTime time);
    void addCurrentAlarmToFavorites();
private:
    void loadHistory(QHBoxLayout *layout);
    QTime current_alarm_time; //currently selected alarm time, can be added to favorites
};

#endif // ALARMHISTORY_H
