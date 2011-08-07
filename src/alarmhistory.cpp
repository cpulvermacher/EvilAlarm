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
    QPushButton *add_favorite_button = new QPushButton(tr("Add %1 to favorites").arg(current_alarm_time.toString(Qt::SystemLocaleShortDate)));

    header_layout->addWidget(help_text);
    header_layout->addStretch();
    header_layout->addWidget(add_favorite_button);

    QGroupBox *favorites_box = new QGroupBox(tr("Favorites"));
    //TODO

    QGroupBox *frequently_used_box = new QGroupBox(tr("Frequently used"));
    QHBoxLayout *frequently_used_layout = new QHBoxLayout();
    //populate list
    loadHistory(frequently_used_layout);
    frequently_used_box->setLayout(frequently_used_layout);

    layout->addLayout(header_layout);
    layout->addWidget(favorites_box);
    layout->addWidget(frequently_used_box);

    connect(add_favorite_button, SIGNAL(clicked()),
        this, SLOT(addCurrentAlarmToFavorites()));
}

AlarmHistory::~AlarmHistory()
{
}

void AlarmHistory::addCurrentAlarmToFavorites()
{
    //TODO
}

void AlarmHistory::setAlarmTime(QTime time)
{
    emit setAlarm(time.hour(), time.minute());
    close();
}

void AlarmHistory::loadHistory(QHBoxLayout *layout)
{
    QSettings settings;
    settings.beginGroup("history");
    QStringList times = settings.childGroups();
    QList<AlarmHistoryItem*> history;
    foreach(QString time, times) {
        if(!settings.contains(time + "/used")) {
            continue; //just to be safe
        }

        int num_used = settings.value(time + "/used").toInt();
        history.append(new AlarmHistoryItem(QTime::fromString(time), num_used, this));
    }
    settings.endGroup();

    //sort by num_used (descending)
    qSort(history.begin(), history.end(), &historyItemUsedMoreOften);

    //get iterator that cuts off after 'num_entries' items
    const int num_entries = 6;
    QList<AlarmHistoryItem*>::iterator cutoff = history.begin();
    for(int i = 0; i < num_entries && cutoff != history.end(); i++)
        cutoff++;

    //sort those selected items by time
    qSort(history.begin(), cutoff, &historyItemBefore);

    //and display them
    QList<AlarmHistoryItem*>::iterator i;
    for(i = history.begin(); i != cutoff; i++) {
        AlarmHistoryItem *item = *i;
        item->updateItem(); //make sure we're using the correct normalization

        layout->addWidget(item);
        connect(item, SIGNAL(selected(QTime)),
                this, SLOT(setAlarmTime(QTime)));
    }

    //delete unused history items (or they'll end up still being shown as they're children of this dialog)
    while(cutoff != history.end()) {
        delete *cutoff;
        cutoff++;
    }

    //add some space on the right side if there's few buttons
    layout->addStretch();
}
