#include "alarmhistory.h"
#include "ui_alarmhistory.h"

AlarmHistory::AlarmHistory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlarmHistory)
{
    ui->setupUi(this);
}

AlarmHistory::~AlarmHistory()
{
    delete ui;
}
