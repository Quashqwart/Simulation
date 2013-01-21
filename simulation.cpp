#include "simulation.h"
#include <math.h>
#include <iostream>

Simulation::Simulation(int step){
    interval = step;
    points = 0;
    std::vector<Point> a;
    pointsList = a;
}

void Simulation::makeStep(){
    double c=10;
    //int m=100000000;
    //for(std::vector<Point>::iterator it =  pointsList.begin(); it != pointsList.end(); it++){
    //    for(std::vector<Point>::iterator jt =  pointsList.begin(); jt != pointsList.end(); jt++){
    //        double dx = jt->x()-it->x();
    //        double dy = jt->y()-it->y();
    //        double d = sqrt(dx*dx+dy*dy);
    //        m=std::min(m,d);
    //    }
    //}
    //if (m<10) c=25;
    //if (m<1) c=50;
    for (int i=0; i<c; i++){
        this->recalculateSpeed(1/c);
        this->recalculatePosition(1/c);
    }
}

void Simulation::recalculatePosition(double dt){
    for(std::vector<Point>::iterator it =  pointsList.begin(); it != pointsList.end(); it++){
        (*it).move(dt);
    }

}

void Simulation::recalculateSpeed(double dt){
    for(std::vector<Point>::iterator it =  pointsList.begin(); it != pointsList.end(); it++){
        for(std::vector<Point>::iterator jt =  pointsList.begin(); jt != pointsList.end(); jt++){
            if (it == jt) continue;
            double dx = jt->x()-it->x();
            double dy = jt->y()-it->y();
            double d = sqrt(dx*dx+dy*dy);
            if (d <= 3) d = 3;
            it->changeVelocity(dt*(jt->getMass())*dx/d/d/d,
                               dt*(jt->getMass())*dy/d/d/d);
        }
    }
}

void Simulation::addPoint(int x, int y, double vx, double vy, double mass){
    //std::cerr<<x<<" "<<y<<" "<<vx<<" "<<vy<<" "<<mass<<std::endl;
    pointsList.push_back(Point(x, y, vx, vy, mass, points++, QColor(0, 0, 0)));
}

void Simulation::addPoint(Point P){
    pointsList.push_back(P);
}

Point Simulation::getPoint(unsigned int i){
    if (pointsList.size() > i) return pointsList[i];
    return Point(0, 0, 0, 0, 0, points++, QColor(0, 0, 0));
}

Simulation::~Simulation(){};

Point::Point(){}

Point::Point(double x, double y, double ivx, double ivy, double massi, int n, QColor colori):QPointF(x, y){
    id = n;
    vx = ivx;
    vy = ivy;
    mass = 0;
    changeMass(massi);
    color = colori;
}

void Point::move(double dt){
    this->setX(this->x() + this->getVX()*dt*0.003);
    this->setY(this->y() + this->getVY()*dt*0.003);
}

void Point::changeVelocity(double dvx, double dvy){
    vx += dvx; vy += dvy;
}

void Point::changeMass(double massi){
    mass = massi;
}

double Point::getVX(){return vx;}
double Point::getVY(){return vy;}
double Point::getMass(){return mass;}
QColor Point::getColor(){return color;}

void Point::setVX(double i){vx=i;}
void Point::setVY(double i){vy=i;}
void Point::setMass(double i){mass=i;}
void Point::setColor(QColor i){color=i;}


