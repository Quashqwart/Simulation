#include "widget.h"
#include "simulation.h"
#include <QtGui>
#include <iostream>
#include <vector>

Widget::Widget(QWidget *parent, int sxi, int syi, double scalei, bool drawpath) :
    QGLWidget(parent)
{
    std::vector<Point> a;
    points = a;
    std::vector<QPainterPath> b;
    paths = b;
    sx = sxi; sy = syi;
    setFixedSize(sx, sy);
    drawPath = drawpath;
    basescale = scalei;
    moving = false;
    first = false;
    dx = 450, dy = 275;
    cnt = 0;
}


void Widget::reset(){
    points.clear();
    paths.clear();
    scale = basescale;
    dx = 450; dy = 275;
    this->update();
}

void Widget::drawPoint(Point Pt){
    if (Pt.id < points.size()){
        paths[Pt.id].lineTo(Pt);
        points[Pt.id] = Pt;
    }
    else{
        QPainterPath n;
        n.moveTo(Pt);
        paths.push_back(n);
        points.push_back(Pt);
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mx = event->x();
        my = event->y();
        moving = true;
    }
    this->repaint();
}

void Widget::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton && moving) {
        dx += event->x()-mx;
        dy += event->y()-my;
        mx = event->x();
        my = event->y();
    }
    this->repaint();
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && moving) {
        dx += event->x()-mx;
        dy += event->y()-my;
        moving = false;
    }
    this->repaint();
}

void Widget::wheelEvent(QWheelEvent *event){
    int x = event->x();
    int y = event->y();
    double nscale;
    if (event->delta() > 0){
        nscale = scale/0.95;
    }
    else{
        nscale = scale*0.95;
    }
    double k = ((nscale/scale)-1);
    int z = x*k-dx*k;
    int p = y*k-dy*k;
    dx -= z;
    dy -= p;
    scale = nscale;
    first = true;
    this->repaint();
}

void Widget::paintEvent(QPaintEvent *){
    QPainter painter;
    painter.begin(this);
    double s1 = scale, s2 = scale;
    painter.scale(s1, s2);
    painter.translate(dx/scale, dy/scale);
    std::vector<QPainterPath>::iterator jt = paths.begin();
    for(std::vector<Point>::iterator it = points.begin(); it != points.end(); it++, jt++){
        painter.setPen((*it).getColor());
        painter.drawEllipse((*it), 3/scale, 3/scale);
        painter.drawPath(*jt);
    }
}
