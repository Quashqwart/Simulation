#ifndef WIDGET_H
#define WIDGET_H

#include <simulation.h>
#include <QWidget>
#include <QGLWidget>
#include <vector>
#include <QColor>
#include <QPainter>
#include <QPainterPath>

class Widget : public QGLWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0, int x = 200, int y = 200, double scale = 0.3, bool drawpath = true);
    void drawPoint(Point);
    void reset();


private:
    bool drawPath;
    double scale, basescale;
    bool moving;
    int sx, sy, cnt;
    int mx, my, dx, dy;
    std::vector<QPainterPath> paths;
    std::vector<Point> points;
    bool first;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *);
    
signals:
    
public slots:
};

#endif // WIDGET_H
