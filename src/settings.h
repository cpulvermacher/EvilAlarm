#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtGui>
#include <QMaemo5ValueButton>

//default values
const QString SOUND_FILE = "/usr/share/sounds/ui-clock_alarm_default.aac";
const float MAX_VOLUME = 80;
const int INACTIVITY_TIMEOUT = 4; //seconds before noise will restart after device has stopped moving
const int ALARM_TIMEOUT = 5; //minutes the device is armed after initial alarm
const int SNOOZE_TIME = 10; //minutes
const int NUM_SNOOZE_MAX = 1;
const bool FULLSCREEN = false;
const bool USE_VIBRATION = false;

namespace Ui
{
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();
    void save();
protected:
    void closeEvent(QCloseEvent*);
private slots:
    void clearHistory();
    void pickSoundFile();
    void moduleChanged();
private:
    QSettings settings;
    QStackedLayout *module_settings_layout;
    QComboBox *module;
    QMaemo5ValueButton *sound_filename;
    QSlider *volume;
    QSpinBox *alarm_timeout;
    QSpinBox *inactivity_timeout;
    QSpinBox *snooze_time;
    QSpinBox *num_snooze_max;
    QCheckBox *fullscreen;
    QCheckBox *prevent_device_lock;
    QCheckBox *vibration;
    QPushButton *clear_history;
    Ui::Settings *ui;
};

#endif // SETTINGS_H
