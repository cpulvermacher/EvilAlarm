#include "selectalarmtype.h"
#include "module_list.h"

#include <QtGui>

SelectAlarmType::SelectAlarmType(QWidget *parent) :
    QDialog(parent),
    module_list(new QListWidget(this))
{
    setWindowTitle(tr("Select Alarm Type"));
    resize(800, 250); //default height is just a bit too low

    module_list->addItems(ModuleList::availableModules());

    //preselect current module
    QSettings settings;
    const QList<QListWidgetItem* > current_module_list = module_list->findItems(settings.value("module").toString(), Qt::MatchFixedString); //case insensitive search
    if(!current_module_list.empty())
        current_module_list.first()->setSelected(true);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(module_list);
    setLayout(layout);

    connect(module_list, SIGNAL(itemActivated(QListWidgetItem*)),
        this, SLOT(selected(QListWidgetItem*)));
}

SelectAlarmType::~SelectAlarmType()
{
    delete module_list;
}

void SelectAlarmType::selected(QListWidgetItem* item)
{
    QSettings settings;
    settings.setValue("module", item->text());
    settings.sync();

    accept(); //close dialog
}
