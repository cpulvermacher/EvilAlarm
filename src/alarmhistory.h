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
    void removeAlarm(QTime time);
    void addCurrentAlarmToFavorites();
private:
    //load up to 'max_items' alarms in group 'listname' into 'list_layout'
    void loadAlarmList(QString listname, QHBoxLayout *list_layout, int max_items = 0);

    QPushButton *add_to_favorites_button;
    QHBoxLayout *favorites_layout;
    QHBoxLayout *history_layout;
    QTime current_alarm_time; //currently selected alarm time, can be added to favorites
};

#endif // ALARMHISTORY_H
