#ifndef SIMULATION_H
#define SIMULATION_H

#include <QPointF>
#include <vector>
#include <QColor>
#include <QMetaType>

class Point : public QPointF{
private:
    double vx, vy;
    QColor color;
    double mass;

    void changeMass(double);

public:
    unsigned int id;
    Point();
    Point(double, double, double, double, double, int, QColor color);
    void changeVelocity(double, double);
    void move(double);

    double getMass();
    double getX();
    double getY();
    double getVX();
    double getVY();
    void setVX(double);
    void setVY(double);
    void setMass(double);
    void setColor(QColor);
    QColor getColor();
};
Q_DECLARE_METATYPE(Point)


class Simulation
{
private:
    int interval, points;
    std::vector<Point> pointsList;

public:
    Simulation(int);
    void addPoint(int, int, double, double, double);
    void addPoint(Point);
    Point getPoint(unsigned int);
    void makeStep();
    ~Simulation();
private:
    void recalculateSpeed(double);
    void recalculatePosition(double);
};

#endif // SIMULATION_H
