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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtMaemo5>
#include <QtGui>

class AlarmHistoryItem;

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
private slots:
	void about();
	void showSettings();
	void testAlarm();
	void toggleAlarm();
	void clearHistory();
	void setAlarmTime(QTime time);
private:
	void reloadHistory();

	QMaemo5ValueButton *time_button;
	QMaemo5TimePickSelector *time_picker;
	QCheckBox *activate_alarm;
	QList<AlarmHistoryItem* > history;
	QHBoxLayout* history_layout;
	QScrollArea* history_scrollarea;
	QTimer timer;
};
#endif
