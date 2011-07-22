#include "settings.h"
#include "module_list.h"
#include "module_settings.h"

#include "ui_settings.h"

#include <iostream>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    //basic settings 
    int row = 0;
    QLabel *snooze_time_label = new QLabel(tr("Snooze time"));
    snooze_time = new QSpinBox();
    snooze_time->setSuffix(" min");
    snooze_time->setRange(1, 60);
    snooze_time->setValue(settings.value("snooze_time", SNOOZE_TIME).toInt());
    ui->basic->addWidget(snooze_time_label, row, 0);
    ui->basic->addWidget(snooze_time, row, 1);
    row++;

    QLabel *num_snooze_max_label = new QLabel(tr("Max. number of snoozes (0 to disable snooze)"));
    num_snooze_max = new QSpinBox();
    num_snooze_max->setValue(settings.value("num_snooze_max", NUM_SNOOZE_MAX).toInt());
    ui->basic->addWidget(num_snooze_max_label, row, 0);
    ui->basic->addWidget(num_snooze_max, row, 1);
    row++;

    ui->basic->addWidget(new QLabel("<center>___________________________________________________</center>"), row, 0, 1, 2);
    row++;

    sound_filename = new QMaemo5ValueButton("Alarm sound", this);
    sound_filename->setValueText(settings.value("sound_filename", SOUND_FILE).toString());
    volume = new QSlider(Qt::Horizontal, this);
    volume->setRange(10, 100);
    volume->setValue(settings.value("max_volume", MAX_VOLUME).toInt());
    volume->setMinimumWidth(100);
    ui->basic->addWidget(sound_filename, row, 0);
    ui->basic->addWidget(volume, row, 1);
    connect(sound_filename, SIGNAL(clicked()),
            this, SLOT(pickSoundFile()));
    row++;

    vibration = new QCheckBox(tr("Enable vibration"));
    vibration->setChecked(settings.value("use_vibration", USE_VIBRATION).toBool());
    ui->basic->addWidget(vibration, row, 0, 1, 2);
    row++;


    //alarm type settings
    row=0;
    QLabel *module_label = new QLabel(tr("Alarm type"));
    module = new QComboBox(this);
    module->addItems(ModuleList::availableModules());
    int current_idx = module->findText(settings.value("module").toString(), Qt::MatchFixedString); //case insensitive search
    if(current_idx == -1) //invalid module, use default
			current_idx = module->findText("Normal");
    module->setCurrentIndex(current_idx);
    ui->alarmtype->addWidget(module_label, row, 0);
    ui->alarmtype->addWidget(module, row, 1);
    row++;

    module_settings_layout = new QStackedLayout();
    ui->alarmtype->addLayout(module_settings_layout, row, 0, 1, 2);
    foreach(QString modulename, ModuleList::availableModules()) {
			QWidget *widget = ModuleList::getSettingsInstance(modulename, this);
			if(!widget)
				widget = new QWidget(this); //module not configurable
			module_settings_layout->addWidget(widget);
    }
    connect(module, SIGNAL(activated(int)),
            module_settings_layout, SLOT(setCurrentIndex(int)));
    connect(module, SIGNAL(activated(int)),
            this, SLOT(moduleChanged()));
    module_settings_layout->setCurrentIndex(current_idx);
    moduleChanged(); //manually update after loading settings
    row++;


    QLabel *alarm_timeout_label = new QLabel(tr("Completely shut down alarm after"));
    alarm_timeout = new QSpinBox();
    alarm_timeout->setSuffix(" min");
    alarm_timeout->setRange(1, 60);
    alarm_timeout->setValue(settings.value("alarm_timeout", ALARM_TIMEOUT).toInt());
    ui->alarmtype->addWidget(alarm_timeout_label, row, 0);
    ui->alarmtype->addWidget(alarm_timeout, row, 1);
    row++;

    QLabel *inactivity_timeout_label = new QLabel(tr("Restart alarm if device not used for"));
    inactivity_timeout = new QSpinBox();
    inactivity_timeout->setSuffix(" s");
    inactivity_timeout->setValue(settings.value("inactivity_timeout", INACTIVITY_TIMEOUT).toInt());
    ui->alarmtype->addWidget(inactivity_timeout_label, row, 0);
    ui->alarmtype->addWidget(inactivity_timeout, row, 1);
    row++;


    //advanced settings
    row=0;
    fullscreen = new QCheckBox(tr("Disable multi-tasking (including Phone app)"));
    fullscreen->setChecked(settings.value("fullscreen", FULLSCREEN).toBool());
    ui->advanced->addWidget(fullscreen);//, row, 0, 1, 2);
    row++;

    prevent_device_lock = new QCheckBox(tr("Prevent device lock (aka. \"Secure Device\")"));
    prevent_device_lock->setChecked(settings.value("prevent_device_lock", false).toBool());
    ui->advanced->addWidget(prevent_device_lock);//, row, 0, 1, 2);
    row++;
}

Settings::~Settings()
{
    delete ui;
}
void Settings::closeEvent(QCloseEvent*) { save(); }

void Settings::pickSoundFile()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Choose Sound File"), sound_filename->valueText(), "Sound Files(*.wav *.mp3 *.ogg *.aac)");
    if(!name.isEmpty())
        sound_filename->setValueText(name);
}

void Settings::save()
{
    std::cout << "Saving settings\n";
    settings.setValue("module", module->currentText());
    settings.setValue("sound_filename", sound_filename->valueText());
    settings.setValue("max_volume", volume->value());
    settings.setValue("use_vibration", vibration->isChecked());
    settings.setValue("alarm_timeout", alarm_timeout->value());
    settings.setValue("inactivity_timeout", inactivity_timeout->value());
    settings.setValue("snooze_time", snooze_time->value());
    settings.setValue("num_snooze_max", num_snooze_max->value());
    settings.setValue("fullscreen", fullscreen->isChecked());
    settings.setValue("prevent_device_lock", prevent_device_lock->isChecked());
    settings.sync();

    //save module settings
    for(int i = 0; i < module_settings_layout->count(); i++) {
        ModuleSettings *module_settings = qobject_cast<ModuleSettings *>(module_settings_layout->itemAt(i)->widget());
        if(module_settings != 0)
            module_settings->save();
    }
}

void Settings::moduleChanged()
{
    //deactivate options not used by the "Normal" module
    bool normal_module = module->currentText().toLower() == "normal";
    inactivity_timeout->setEnabled(!normal_module);
}
