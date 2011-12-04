#ifndef SELECTALARMTYPE_H
#define SELECTALARMTYPE_H

#include <QDialog>
#include <QListWidget>


class SelectAlarmType : public QDialog
{
    Q_OBJECT

public:
    explicit SelectAlarmType(QWidget *parent = 0);
    ~SelectAlarmType();

public slots:
    void selected(QListWidgetItem* item);

private:
    QListWidget *module_list;
};

#endif // SELECTALARMTYPE_H
