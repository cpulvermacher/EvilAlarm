/*
    Blubbels
    Copyright (C) 2007-2010 Christian Pulvermacher

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
#ifndef BLUBBELS_H
#define BLUBBELS_H

#include <QList>
#include <QPixmap>
#include <QWidget>

class QMouseEvent;
class QSound;
class QTimer;

struct Box {
	Box(int x = -1, int y = -1): x(x), y(y) { }
	bool operator==(const Box &o) const { return x == o.x && y == o.y; }

	int x,y;
};

struct Bubble {
	Bubble(short color = 0):
		color(color), xoffset(0), yoffset(0), xspeed(0), yspeed(0), scale_step(0)
	{
		//used for animations, should be enough for gridsize <= 16
		static short i = 0;
		id = i++;
	}
	short color; // 0 means empty

	//for animations
	float xoffset, yoffset; //in negative x/y-direction, 0 if none - in grid units
	float xspeed, yspeed;
	int scale_step; // in [0..num_scale_steps-1], 0 being full size
	short id;
};


class GameWidget : public QWidget {
	Q_OBJECT
public:
	GameWidget(QWidget* parent = 0);
public slots:
	void restart();
	void undo();
signals:
	void enableUndo(bool on);
	void newScore(int score);
protected: //overrides
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
private slots:
	void animationStep();
private:
	void checkGameOver();
	void compressBubbles();
	void getConnectedBubbles(Box pos, QList<Box> &list);
	Box getGridPos(int x, int y);

	const static int grid_size = 11;
	const static int num_colors = 5;
	const static int num_scale_steps = 10;

	Bubble grid[grid_size][grid_size];
	Bubble old_grid[grid_size][grid_size];
	QList<Box> selection;
	int bubblesize, margin, xoff, yoff;
	int score, oldscore;
	bool use_animations;
	QPixmap bubblePixmap[num_colors+1][num_scale_steps]; // [color][scale], color 0 is empty, scale 0 is full size

	QSound *pop;
	QTimer *timer; //for animations
};
#endif
