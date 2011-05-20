#ifndef MODULE_SETTINGS_H
#define MODULE_SETTINGS_H

#include <QWidget>

class ModuleSettings : public QWidget {
	Q_OBJECT
public:
	ModuleSettings(QWidget *parent): QWidget(parent) { }
	virtual ~ModuleSettings() { }
	virtual void save() = 0;
};

#endif
