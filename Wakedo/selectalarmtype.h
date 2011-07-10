#ifndef SELECTALARMTYPE_H
#define SELECTALARMTYPE_H

#include <QDialog>

namespace Ui {
    class SelectAlarmType;
}

class SelectAlarmType : public QDialog
{
    Q_OBJECT

public:
    explicit SelectAlarmType(QWidget *parent = 0);
    ~SelectAlarmType();

private:
    Ui::SelectAlarmType *ui;
};

#endif // SELECTALARMTYPE_H
