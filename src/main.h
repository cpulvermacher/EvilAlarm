/*
    EvilAlarm
    Copyright (C) 2010 Christian Pulvermacher

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#ifndef MAIN_H
#define MAIN_H

#include <QtMaemo5>
#include <QtGui>

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
protected:
	void closeEvent(QCloseEvent *ev);
private slots:
	void about();
	void showPreferences();
	void toggleTimer();
	void wake();
private:
	QMaemo5ValueButton *time_button;
	QMaemo5TimePickSelector *time_picker;
	QPushButton *activate_button;

	QTimer timer;
};
#endif
