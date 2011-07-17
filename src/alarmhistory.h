#ifndef ALARMHISTORY_H
#define ALARMHISTORY_H

#include <QDialog>

namespace Ui {
    class AlarmHistory;
}

class AlarmHistory : public QDialog
{
    Q_OBJECT

public:
    explicit AlarmHistory(QWidget *parent = 0);
    ~AlarmHistory();

private:
    Ui::AlarmHistory *ui;
};

#endif // ALARMHISTORY_H
