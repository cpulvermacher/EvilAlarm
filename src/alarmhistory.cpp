#include "alarmhistory.h"
#include "alarmhistoryitem.h"

#include <QtGui>


//sort helpers
bool historyItemUsedMoreOften(const AlarmHistoryItem* a, const AlarmHistoryItem* b)
{
    return a->numUsed() > b->numUsed();
}

bool historyItemBefore(const AlarmHistoryItem *a, const AlarmHistoryItem *b)
{
    return a->time() < b->time();
}


AlarmHistory::AlarmHistory(QWidget *parent, int hours, int minutes) :
    QDialog(parent),
    current_alarm_time(QTime(hours, minutes))
{
    QSettings settings;

    setWindowTitle(tr("Previous Alarms"));
    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *header_layout = new QHBoxLayout();
    QLabel *help_text = new QLabel(tr("Tap time to set alarm!"));
    add_to_favorites_button = new QPushButton(tr("Add %1 to favorites").arg(current_alarm_time.toString(Qt::SystemLocaleShortDate)));

    header_layout->addWidget(help_text);
    header_layout->addStretch();
    header_layout->addWidget(add_to_favorites_button);

    QGroupBox *favorites_box = new QGroupBox(tr("Favorites"));
    favorites_layout = new QHBoxLayout();
    favorites_box->setLayout(favorites_layout);

    QGroupBox *history_box = new QGroupBox(tr("Frequently used"));
    history_layout = new QHBoxLayout();
    history_box->setLayout(history_layout);

    layout->addLayout(header_layout);
    layout->addWidget(favorites_box);
    layout->addWidget(history_box);

    connect(add_to_favorites_button, SIGNAL(clicked()),
            this, SLOT(addCurrentAlarmToFavorites()));

    //populate lists
    loadAlarmList("favorites", favorites_layout);
    loadAlarmList("history", history_layout, NUM_HISTORY_ITEMS);
}

AlarmHistory::~AlarmHistory()
{
    delete favorites_layout;
    delete history_layout;
}

void AlarmHistory::addCurrentAlarmToFavorites()
{
    QSettings settings;
    settings.beginGroup("favorites");
    settings.setValue(QString("%1/used").arg(current_alarm_time.toString()), 1);
    settings.endGroup();

    settings.sync();

    //reload list
    loadAlarmList("favorites", favorites_layout);
}

void AlarmHistory::removeAlarm(QTime time)
{
    QWidget *sender_wdgt = dynamic_cast<QWidget*>(sender());
    Q_ASSERT(sender_wdgt);

    //did the selected item come from history or favorites?
    QString listname;
    if(history_layout->indexOf(sender_wdgt) != -1)
        listname = "history";
    else
        listname = "favorites";

    //remove item
    QSettings settings;
    settings.beginGroup(listname);
    settings.remove(time.toString());
    settings.endGroup();
    settings.sync();

    //reload list
    if(listname == "favorites")
        loadAlarmList("favorites", favorites_layout);
    else
        loadAlarmList("history", history_layout, NUM_HISTORY_ITEMS);
}

void AlarmHistory::setAlarmTime(QTime time)
{
    emit setAlarm(time.hour(), time.minute());
    close();
}


void AlarmHistory::loadAlarmList(QString listname, QHBoxLayout *list_layout, int max_items)
{
    //remove any old items in layout
    QLayoutItem *child;
    while ((child = list_layout->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }

    QSettings settings;
    settings.beginGroup(listname);
    QStringList times = settings.childGroups();
    settings.endGroup();
    QList<AlarmHistoryItem*> alarm_list;
    foreach(QString time, times) {
        //take #used from history (favorites don't store usage themselves)
        const int num_used = settings.value(QString("history/%1/used").arg(time), 1).toInt();

        if(num_used > 0)
            alarm_list.append(new AlarmHistoryItem(QTime::fromString(time), num_used, this));
    }

    //don't show "Add to Favorites" button when already in list
    if(listname == "favorites")
        add_to_favorites_button->setVisible(!times.contains(current_alarm_time.toString()));


    //get iterator that cuts off after 'max_items' items
    QList<AlarmHistoryItem*>::iterator cutoff = alarm_list.begin();
    if(max_items > 0) {
        //sort by num_used (descending)
        qSort(alarm_list.begin(), alarm_list.end(), &historyItemUsedMoreOften);

        for(int i = 0; i < max_items && cutoff != alarm_list.end(); i++)
            cutoff++;
    } else {
        cutoff = alarm_list.end();
    }

    //sort those selected items by time
    qSort(alarm_list.begin(), cutoff, &historyItemBefore);

    //and display them
    QList<AlarmHistoryItem*>::iterator i;
    for(i = alarm_list.begin(); i != cutoff; i++) {
        AlarmHistoryItem *item = *i;

        list_layout->addWidget(item);
        connect(item, SIGNAL(selected(QTime)),
                this, SLOT(setAlarmTime(QTime)));
        //without QueuedConnection, execution would continue in the destoyed object!
        connect(item, SIGNAL(remove(QTime)),
                this, SLOT(removeAlarm(QTime)), Qt::QueuedConnection);
    }

    //delete unused alarm items (or they'll end up still being shown as they're children of this dialog)
    while(cutoff != alarm_list.end()) {
        delete *cutoff;
        cutoff++;
    }

    //add some space on the right side if there's few buttons
    list_layout->addStretch();
}
