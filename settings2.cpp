#include "settings2.h"
#include "ui_settings2.h"

Settings2::Settings2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings2)
{
    ui->setupUi(this);
}

Settings2::~Settings2()
{
    delete ui;
}
