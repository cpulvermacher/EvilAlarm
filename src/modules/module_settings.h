#ifndef MODULE_SETTINGS_H
#define MODULE_SETTINGS_H

#include <QWidget>

class ModuleSettings : public QWidget {
	Q_OBJECT
public:
	ModuleSettings(QWidget *parent): QWidget(parent) { };
	void save() { };
};


#endif
