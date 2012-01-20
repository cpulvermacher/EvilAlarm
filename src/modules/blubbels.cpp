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
#include "blubbels.h"

#include <QColor>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QSettings>
#include <QTimer>


const QColor bubbleColor[] = {
    Qt::white, //"empty", shouldn't be shown
    Qt::green,
    Qt::blue,
    Qt::red,
    QColor(255, 180, 0),
    Qt::gray
};

const float initial_speed = 0.07;


GameWidget::GameWidget(QWidget* parent) :
    QWidget(parent),
    bubblesize(1),
    margin(1),
    use_animations(true),
    timer(new QTimer(this))
{
    setMouseTracking(true);

    setAttribute(Qt::WA_OpaquePaintEvent);
    setMinimumSize(90, 90);

    connect(timer, SIGNAL(timeout()),
            this, SLOT(animationStep()));
    timer->setInterval(50);

    srand(time(0));
    restart();
}


//translates cursor position into grid coordinates
Box GameWidget::getGridPos(int x, int y)
{
    Box result; //invalid default value
    //make coords relative to bubbles
    x -= xoff;
    y -= yoff;

    if(x >=0 and y >= 0
            and (x-margin/2) / (bubblesize+margin) < grid_size and (y-margin/2) / (bubblesize+margin) < grid_size
            and grid[(x-margin/2)/(bubblesize+margin)][(y-margin/2)/(bubblesize+margin)].color) {
        result.x = (x-margin/2) / (bubblesize+margin);
        result.y = (y-margin/2) / (bubblesize+margin);
    }
    return result;
}


//advances all animations by one frame
void GameWidget::animationStep()
{
    bool animations_found = false;
    for(int x = 0; x < grid_size; x++) {
        for(int y = 0; y < grid_size; y++) {
            if(grid[x][y].yoffset) {
                grid[x][y].yoffset -= grid[x][y].yspeed;
                grid[x][y].yspeed *= 1.7;

                if((grid[x][y].yspeed > 0 and grid[x][y].yoffset <= 0)
                        or (grid[x][y].yspeed < 0 and grid[x][y].yoffset >= 0))
                    grid[x][y].yoffset = 0;
                else
                    animations_found = true;
            }
            if(grid[x][y].xoffset) {
                grid[x][y].xoffset -= grid[x][y].xspeed;
                grid[x][y].xspeed *= 1.7;

                if((grid[x][y].xspeed > 0 and grid[x][y].xoffset <= 0)
                        or (grid[x][y].xspeed < 0 and grid[x][y].xoffset >= 0))
                    grid[x][y].xoffset = 0;
                else
                    animations_found = true;
            }
            if(grid[x][y].scale_step) {
                grid[x][y].scale_step--;
                if(grid[x][y].scale_step != 0)
                    animations_found = true;
            }
        }
    }

    if(!animations_found)
        timer->stop();
    update();
}


//let bubbles fall down / move stacks to right
void GameWidget::compressBubbles()
{
    for(int x = grid_size-1; x >= 0; x--) {
        int hole = -1;
        int holes = 0;
        bool bubble_found = false;
        for(int y = 0; y <= grid_size; y++) {
            if(y == grid_size or grid[x][y].color) { //not empty
                if(holes) { //hole found, move bubbles down
                    for(int i = hole; i > 0; i--) {
                        grid[x][i] = grid[x][i-holes];

                        if(use_animations) {
                            grid[x][i].yoffset = holes;
                            grid[x][i].yspeed = initial_speed;
                        }
                    }
                    for(int i = 0; i < holes; i++)
                        grid[x][i] = Bubble();
                }
                bubble_found = true;
                hole = -1;
                holes = 0;
            } else if(bubble_found) {
                hole = y;
                holes++;
            }
        }
    }

    for(int x = grid_size-1; x >= 0; x--) {
        if(!grid[x][grid_size-1].color) { //collumn empty
            //shift remaining collums
            int x2 = x;
            while(--x2 >= 0) {
                for(int y = 0; y < grid_size; y++) {
                    grid[x2+1][y] = grid[x2][y];

                    if(use_animations) {
                        grid[x2+1][y].xoffset += 1;
                        grid[x2+1][y].xspeed = initial_speed;
                    }
                }
            }

            //add new collumn
            const int height = int(double(rand())/(RAND_MAX + 1.0)*grid_size);
            for(int y = grid_size-1; y >= height; y--) {
                grid[0][y] = Bubble(double(rand())/(RAND_MAX + 1.0)*num_colors+1);

                if(use_animations) {
                    grid[0][y].scale_step = num_scale_steps-1;
                }
            }

            //erase remains of old collumn
            for(int y = height-1; y >= 0; y--)
                grid[0][y] = Bubble();

            x++; //make sure to run over collumn again
        }
    }
    if(use_animations)
        timer->start();

    update();
    checkGameOver();
}


//returns list of all bubbles connected to pos
void GameWidget::getConnectedBubbles(Box pos, QList<Box> &list)
{
    int x = pos.x;
    int y = pos.y;
    const short color = grid[x][y].color;
    if(!color) //nothing to select, exit
        return;

    list << pos;

    x--; //left
    if(x >= 0 and color == grid[x][y].color and !list.contains(Box(x, y))) {
        getConnectedBubbles(Box(x, y), list);
    }
    x += 2; //right
    if(x < grid_size and color == grid[x][y].color and !list.contains(Box(x, y))) {
        getConnectedBubbles(Box(x, y), list);
    }
    x--;
    y--; //up
    if(y >= 0 and color == grid[x][y].color and !list.contains(Box(x, y))) {
        getConnectedBubbles(Box(x, y), list);
    }
    y += 2; //down
    if(y < grid_size and color == grid[x][y].color and !list.contains(Box(x, y))) {
        getConnectedBubbles(Box(x, y), list);
    }
}


void GameWidget::checkGameOver()
{
    for(int x = 0; x < grid_size; x++) {
        for(int y = 0; y < grid_size; y++) {
            QList<Box> connected;
            getConnectedBubbles(Box(x,y), connected);
            if(connected.size() > 1)
                return; //found cluster, continue game
        }
    }
    //assertion: no connected bubbles left in grid

    emit newScore(score);

    restart();
}


void GameWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons())
        return; //don't change selection while mouse button is down

    const Box hovered = getGridPos(event->x(), event->y());
    if(selection.contains(hovered))
        return; //cursor in old selection, nothing new

    selection.clear();
    if(hovered.x != -1) {
        getConnectedBubbles(hovered, selection);

        if(selection.size() == 1)
            selection.clear(); //don't select single bubbles
    }
    update();
}


void GameWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(selection.size() < 2 or !selection.contains(getGridPos(event->x(), event->y())))
        return; //nothing selected or mouse pointer not in selection

    //backup
    oldscore = score;
    for(int x = 0; x < grid_size; x++)
        for(int y = 0; y < grid_size; y++)
            old_grid[x][y] = grid[x][y];

    foreach(Box pos, selection) {
        if(use_animations)
            old_grid[pos.x][pos.y].scale_step = num_scale_steps-1;

        grid[pos.x][pos.y] = Bubble(); //remove bubbles
    }

    score += selection.size()*selection.size();
    selection.clear();

    emit enableUndo(true);
    emit activity();

    compressBubbles();

#ifndef Q_WS_MAEMO_5
    mouseMoveEvent(event); //draw selection if there's a new set of connected bubbles
#endif
}


void GameWidget::resizeEvent(QResizeEvent* /*event*/)
{
    const int old_bubblesize = bubblesize;
    const int max_size = qMin(width(), height());

    margin = max_size / 70; //separates bubbles from each other and the widget border
    bubblesize = (max_size - margin*(grid_size+1)) / grid_size;

    //center grid
    xoff = (width() - margin*(grid_size+1) - bubblesize*grid_size)/2;
    yoff = (height() -margin*(grid_size+1) - bubblesize*grid_size)/2;

    if(bubblesize != old_bubblesize) { //generate new pixmaps
        for(int i = 1; i <= num_colors; i++) {
            for(int j = 0; j < num_scale_steps; j++) {
                bubblePixmap[i][j] = QPixmap(bubblesize, bubblesize); //pixmaps have same size, graphic may be tinier
                bubblePixmap[i][j].fill(Qt::transparent);

                const qreal scaled_size = (1 - double(j)/num_scale_steps)*(1 - double(j)/num_scale_steps) * bubblesize;

                QPainter painter(&bubblePixmap[i][j]);
                painter.setPen(Qt::transparent);
                painter.setRenderHint(QPainter::Antialiasing, true);

                QRadialGradient radialGrad(QPointF(0.3*bubblesize, 0.3*bubblesize), 0.8*scaled_size);
                radialGrad.setColorAt(0, Qt::white);
                radialGrad.setColorAt(0.4, bubbleColor[i]);
                radialGrad.setColorAt(1, bubbleColor[i].darker(700));

                painter.setBrush(QBrush(radialGrad));
                painter.drawEllipse((bubblesize-scaled_size)/2, (bubblesize-scaled_size)/2,
                                    scaled_size, scaled_size);
            }
        }
    }

    update();
}


//start new game, fill grid with bubbles
void GameWidget::restart()
{
    emit enableUndo(false);

    score = 0;
    selection.clear();

    //seed field with random non-empty bubbles (values 1..num_colors)
    for(int x = 0; x < grid_size; x++) {
        for(int y = 0; y < grid_size; y++) {
            grid[x][y] = Bubble(double(rand())/(RAND_MAX+1.0)*num_colors+1);

            if(use_animations) {
                grid[x][y].xoffset = grid_size + (width() - grid_size*(bubblesize+margin))/bubblesize/2;
                grid[x][y].xspeed = initial_speed;
            }
        }
    }

    if(use_animations)
        timer->start();

    update();
}


//undo the last move
void GameWidget::undo()
{
    emit enableUndo(false);

    score = oldscore;
    selection.clear();

    if(use_animations) {
        int shift = 0; //number of new collums in the action we want to undo
        for(int x = grid_size-1; x >= shift; x--) {
            bool not_found = false;
            for(int y = 1; y < grid_size; y++) {
                if(grid[x][y].id == old_grid[x-shift][y].id)
                    break; //nothing changed

                not_found = true;
                for(int y_old = y-1; y_old >= 0; y_old--) { //search upward
                    if(grid[x][y].id == old_grid[x-shift][y_old].id) {
                        old_grid[x-shift][y_old].yoffset = y_old - y;
                        old_grid[x-shift][y_old].yspeed = -initial_speed;

                        not_found = false;
                        break; //ids are unique
                    }
                }

            }

            if(not_found) {//we couldn't find any bubbles in their old collumn
                shift++; //so it must have been the whole collumn that moved
            }

            if(shift > 0) {
                for(int y = 0; y < grid_size; y++) {
                    old_grid[x-shift][y].xoffset = -shift;
                    old_grid[x-shift][y].xspeed = -initial_speed;
                }
            }

            //don't skip collumns
            //TODO: only fixes special case shift=2
            if(not_found and shift > 1) {
                for(int y = 0; y < grid_size; y++) {
                    old_grid[x-shift+1][y].xoffset = -shift;
                    old_grid[x-shift+1][y].xspeed = -initial_speed;
                }
            }
        }

        timer->start();
    }

    for(int x = 0; x < grid_size; x++)
        for(int y = 0; y < grid_size; y++)
            grid[x][y] = old_grid[x][y];

    update();
}


void GameWidget::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), QBrush(Qt::black));
    painter.translate(xoff, yoff);

    painter.setPen(QPen(Qt::black));
    QFont font("Courier");
    font.setPixelSize(20);
    font.setBold(true);
    painter.setFont(font);

    //draw bubbles
    for(int i = 0; i < grid_size; i++) {
        for(int j = 0; j < grid_size; j++) {
            if(!grid[i][j].color) //empty
                continue;

            QRectF target(
                (i-grid[i][j].xoffset) * (bubblesize+margin) + margin,
                (j-grid[i][j].yoffset) * (bubblesize+margin) + margin,
                bubblesize, bubblesize);
            QRectF source(0, 0, bubblesize, bubblesize);
            painter.drawPixmap(target, bubblePixmap[grid[i][j].color][grid[i][j].scale_step], source);
        }
    }


    //show current score
    painter.translate(-xoff, -yoff);
    painter.setBrush(QBrush(QColor(255, 255, 255, 160)));
    QRect score_rect = QRect(-5, -12, 150, 37);

    painter.drawRoundedRect(score_rect, 5, 5);
    painter.drawText(score_rect, Qt::AlignHCenter | Qt::AlignBottom, tr("Score: %1").arg(score));


    //draw selection (if there is one)
    if(selection.empty() or selection.size() == 1)
        return;

    painter.translate(xoff, yoff);
    const short color = grid[selection.first().x][selection.first().y].color;
    painter.setPen(QPen(bubbleColor[color], margin/3, Qt::DotLine, Qt::RoundCap));
    QPainterPath path;

    foreach(Box b, selection) {
        if(b.x == 0 || grid[b.x-1][b.y].color != color) {
            QPointF point(b.x*(bubblesize+margin)+qreal(margin)/2, qreal(b.y*(bubblesize+margin)+margin/2));
            path.moveTo(point);
            point.ry() += bubblesize+margin;
            path.lineTo(point);

        }
        if(b.x == grid_size-1 || grid[b.x+1][b.y].color != color) {
            QPointF point(b.x*(bubblesize+margin)+qreal(margin)*3/2+bubblesize, b.y*(bubblesize+margin)+qreal(margin)/2);
            path.moveTo(point);
            point.ry() += bubblesize+margin;
            path.lineTo(point);
        }
        if(b.y == 0 || grid[b.x][b.y-1].color != color) {
            QPointF point(qreal(b.x*(bubblesize+margin)+margin/2), qreal(b.y*(bubblesize+margin)+margin/2));
            path.moveTo(point);
            point.rx() += bubblesize+margin;
            path.lineTo(point);
        }
        if(b.y == grid_size-1 || grid[b.x][b.y+1].color != color) {
            QPointF point(qreal(b.x*(bubblesize+margin)+margin/2), qreal(b.y*(bubblesize+margin)+margin*3/2+bubblesize));
            path.moveTo(point);
            point.rx() += bubblesize+margin;
            path.lineTo(point);
        }
    }
    painter.drawPath(path);


    //show points awarded for removing selected bubbles
    QString points = QString::number(selection.size() * selection.size());
    const int text_width = points.length()*15;

    QColor pen_color = bubbleColor[color];
    painter.setPen(QPen(pen_color));
    painter.setBrush(QBrush(QColor(0, 0, 0, 200)));

    const int bg_width = text_width+6;
    const int bg_height = 26;
    QPoint center = path.boundingRect().center().toPoint();
    QRect text_rect(center.x()-bg_width/2, center.y()-bg_height/2, bg_width, bg_height);
    painter.drawRoundedRect(text_rect, 4, 4);

    if(pen_color == Qt::blue) //too dark for text
        pen_color = pen_color.lighter(150);
    painter.setPen(QPen(pen_color));
    painter.drawText(text_rect, Qt::AlignCenter, points);
}
