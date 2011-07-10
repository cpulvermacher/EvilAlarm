#include "selectalarmtype.h"
#include "ui_selectalarmtype.h"

SelectAlarmType::SelectAlarmType(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectAlarmType)
{
    ui->setupUi(this);
}

SelectAlarmType::~SelectAlarmType()
{
    delete ui;
}
