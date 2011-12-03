#include "alarmhistoryitem.h"

const int LONG_PRESS_MS = 1000;

AlarmHistoryItem::AlarmHistoryItem(QTime t, int used, QWidget *parent):
    QPushButton(parent),
    alarm_time(t),
    num_used(used)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    setText(alarm_time.toString(Qt::SystemLocaleShortDate));

    connect(this, SIGNAL(clicked()),
            this, SLOT(emitSelected()));

    //implement long press
    button_held_timer.setInterval(LONG_PRESS_MS);
    connect(&button_held_timer, SIGNAL(timeout()),
            this, SLOT(longPress()));
    connect(this, SIGNAL(pressed()),
            &button_held_timer, SLOT(start()));
    connect(this, SIGNAL(released()),
            &button_held_timer, SLOT(stop()));
}

AlarmHistoryItem::~AlarmHistoryItem() { }

void AlarmHistoryItem::longPress()
{
    button_held_timer.stop();
    setDown(false); //prevent clicked() signal from being emitted

    QMenu menu(this);
    menu.addAction(tr("Remove"), this, SLOT(emitRemove()));

    setMenu(&menu);

    showMenu(); //blocks until menu is closed

    //menu is deleted so this doesn't affect the button's look
}

void AlarmHistoryItem::emitRemove() { emit remove(alarm_time); }
void AlarmHistoryItem::emitSelected() { emit selected(alarm_time); }
