#include <QtGui>
#include "window.h"
#include "simulation.h"
#include "widget.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

Window::Window():QWidget(){
    int x = 900, y = 570;
    interval = 1; redraw_interval = 10;
    drawArea = new Widget(this, x, y, 0.3, true);
    drawAreaLabel = new QLabel(tr("Ready"));

    QGridLayout *mainLayout = new QGridLayout;
    QVBoxLayout *btLayout = new QVBoxLayout;
    ptsList = new QListWidget;
    QVBoxLayout *ptsLayout = new QVBoxLayout;
    QGridLayout *ptsCtrLayout = new QGridLayout;


    start = new QPushButton(tr("Start"), this);
    stop = new QPushButton(tr("Stop"), this); stop->setEnabled(false);
    restart = new QPushButton(tr("Restart"), this); restart->setEnabled(false);

    connect(start, SIGNAL(clicked()), this, SLOT(startSim()));
    connect(stop, SIGNAL(clicked()), this, SLOT(stopSim()));
    connect(restart, SIGNAL(clicked()), this, SLOT(restartSim()));
    btLayout->addWidget(start);
    btLayout->addWidget(stop);
    btLayout->addWidget(restart);

    QPushButton *add = new QPushButton(tr("Add"), this);
    QPushButton *del = new QPushButton(tr("Delete"), this);
    QPushButton *edit = new QPushButton(tr("Edit"), this);
    clrBtn = new QPushButton();
    connect(add, SIGNAL(clicked()), this, SLOT(addPoint()));
    connect(del, SIGNAL(clicked()), this, SLOT(deletePoint()));
    connect(edit, SIGNAL(clicked()), this, SLOT(editPoint()));
    connect(clrBtn, SIGNAL(clicked()), this, SLOT(selectColor()));
    connect(ptsList, SIGNAL(clicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));

    filename = new QLineEdit();
    QPushButton *load = new QPushButton(tr("Load"), this);
    connect(load, SIGNAL(clicked()), this, SLOT(loadConf()));
    QPushButton *save = new QPushButton(tr("Save"), this);
    connect(save, SIGNAL(clicked()), this, SLOT(saveConf()));

    ptsCtrLayout->addWidget(add, 0, 0);
    ptsCtrLayout->addWidget(del, 0, 1);
    ptsCtrLayout->addWidget(edit, 0, 2);
    ptsCtrLayout->addWidget(filename,1,0);
    ptsCtrLayout->addWidget(save,1,1);
    ptsCtrLayout->addWidget(load,1,2);
    ptsLayout->addWidget(ptsList);

    cLabel = new QLabel(tr("Coordinate (X, Y):"));
    vLabel = new QLabel(tr("Speed (X, Y):"));
    massLabel = new QLabel(tr("Mass:"));
    colorLabel = new QLabel(tr("Color:"));
    pal = new QPalette;
    pal->setColor(QPalette::Button, QColor(50, 0, 0));
    clrBtn->setPalette(*pal);

    px = new QSpinBox;
    px->setRange(-10000, 10000);
    py = new QSpinBox;
    py->setRange(-10000, 10000);
    pvx = new QSpinBox;
    pvx->setRange(-10000, 10000);
    pvy = new QSpinBox;
    pvy->setRange(-10000, 10000);
    pmass = new QSpinBox;
    pmass->setRange(-10000, 10000);

    ngen = new QSpinBox;
    ngen->setRange(1,10);
    QPushButton *gen = new QPushButton(tr("Gen"), this);
    connect(gen, SIGNAL(clicked()), this, SLOT(genConf()));

    ptsCtrLayout->addWidget(cLabel, 2, 0);
    ptsCtrLayout->addWidget(vLabel, 3, 0);
    ptsCtrLayout->addWidget(massLabel, 4, 0);
    ptsCtrLayout->addWidget(colorLabel, 5, 0);
    ptsCtrLayout->addWidget(px, 2, 1);
    ptsCtrLayout->addWidget(py, 2, 2);
    ptsCtrLayout->addWidget(pvx, 3, 1);
    ptsCtrLayout->addWidget(pvy, 3, 2);
    ptsCtrLayout->addWidget(pmass, 4, 2);
    ptsCtrLayout->addWidget(clrBtn, 5, 2);
    ptsCtrLayout->addWidget(ngen, 6, 0);
    ptsCtrLayout->addWidget(gen, 6, 1);
    ptsLayout->addLayout(ptsCtrLayout);

    mainLayout->addWidget(drawArea, 0, 0);
    mainLayout->addWidget(drawAreaLabel, 1, 0);
    mainLayout->addLayout(btLayout, 2, 0);
    mainLayout->addLayout(ptsLayout, 0, 1);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(step()));

    this->setLayout(mainLayout);
}

void Window::addPoint(){
    QColorDialog clr;
    Point p((double)px->value(), (double)py->value(), (double)pvx->value(), (double)pvy->value(), (double)pmass->value(), ptsList->count(), pal->color(QPalette::Button));
    QVariant c = QVariant::fromValue(p);
    ptsList->addItem(tr("Point %1").arg(p.id+1));
    ptsList->item(ptsList->count()-1)->setData(Qt::UserRole, c);
}

void Window::editPoint(){
    if (ptsList->currentItem() == NULL) return;
    Point p = ptsList->currentItem()->data(Qt::UserRole).value<Point>();
    p.setX((double)px->value());
    p.setY((double)py->value());
    p.setVX((double)pvx->value());
    p.setVY((double)pvy->value());
    p.setMass((double)pmass->value());
    p.setColor(pal->color(QPalette::Button));
    QVariant c = QVariant::fromValue(p);
    ptsList->currentItem()->setData(Qt::UserRole, c);
    p.setVX((double)pvx->value()*0.003);
    p.setVY((double)pvy->value()*0.003);
}

void Window::deletePoint(){
    if (ptsList->currentItem() == NULL) return;
    int id = ptsList->currentItem()->data(Qt::UserRole).value<Point>().id;
    QListWidgetItem* t = ptsList->takeItem(id);
    delete t;
    for(int i = id; i < ptsList->count(); i++){
        Point p = ptsList->item(i)->data(Qt::UserRole).value<Point>();
        p.id--;
        QVariant c = QVariant::fromValue(p);
        ptsList->item(i)->setData(Qt::UserRole, c);
    }
    ptsList->update();
}
void Window::loadConf(){
    QFile file(filename->text());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream fin(&file);
    double x,y,vx,vy,m;
    int n,i,r,g,b;
    ptsList->clear();
    fin>>n;
    for (i=0; i<n; i++){
        fin>>x>>y>>vx>>vy>>m>>r>>g>>b;
        QColor *tcolor=new QColor(r,g,b);
        Point p((double)x, (double)y, (double)vx, (double)vy, (double)m, ptsList->count(), *tcolor);
        QVariant c = QVariant::fromValue(p);
        ptsList->addItem(tr("Point %1").arg(p.id+1));
        ptsList->item(ptsList->count()-1)->setData(Qt::UserRole, c);
    }
}
void Window::genConf(){
    int color[][3]={{255,0,0},{0,170,0},{0,0,255},{170,0,255},{85,0,127},{255,170,0},{255,0,127},{255,50,0},{127, 170,0}};
    srand ( time(NULL) );
    ptsList->clear();
    int n=ngen->value();
    for (int i=0; i<n; i++){
        //cout<<rand()%1001-500<<" "<<rand()%1001-500<<" "<<
        //      rand()%41-20<<" "<<rand()%41-20<<" "<<
        //      rand()%101+100<<" "<<
        //      color[i][0]<<" "<<color[i][1]<<" "<<color[i][2]<<endl;
        QColor *tcolor=new QColor(color[i][0],color[i][1],color[i][2]);
        Point p((double)(rand()%1001-500), (double)(rand()%1001-500),
                (double)(rand()%41-20), (double)(rand()%41-20),
                (double)(rand()%101+100),
                ptsList->count(), *tcolor);
        QVariant c = QVariant::fromValue(p);
        ptsList->addItem(tr("Point %1").arg(p.id+1));
        ptsList->item(ptsList->count()-1)->setData(Qt::UserRole, c);
    }
}
void Window::saveConf(){
    QFile file(filename->text());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream fout(&file);
    fout<<ptsList->count()<<endl;
    for(int i = 0; i < ptsList->count(); i++){
        Point p = ptsList->item(i)->data(Qt::UserRole).value<Point>();
        fout<<p.x()<<" "<<p.y()<<" "<<p.getVX()<<" "<<p.getVY()<<" "<<p.getMass()<<" "
            <<p.getColor().red()<<" "<<p.getColor().green()<<" "<<p.getColor().blue()<<endl;
    }

}

void Window::itemClicked(QModelIndex index){
    Point p = index.data(Qt::UserRole).value<Point>();
    px->setValue(p.x());
    py->setValue(p.y());
    pvx->setValue(p.getVX());
    pvy->setValue(p.getVY());
    pmass->setValue(p.getMass());
    delete pal;
    pal = new QPalette;
    pal->setColor(QPalette::Button, p.getColor());
    clrBtn->setPalette(*pal);
}

void Window::selectColor(){
    QColorDialog clr;
    QColor c = clr.getColor();
    delete pal;
    pal = new QPalette;
    pal->setColor(QPalette::Button, c);
    clrBtn->setPalette(*pal);
    clrBtn->setStyleSheet("QPushButton { background-color : *pal;}");
    clrBtn->repaint();
}

void Window::step(){
    sim->makeStep();

    redraw_interval -= interval;
    if (redraw_interval <= 0){
        redraw_interval = 10;
        simtime++;
        for(int i = 0; i < ptsList->count(); i++){
            drawArea->drawPoint(sim->getPoint(i));
        }
        drawAreaLabel->setText(tr("Simulation time: %1,%2").arg(simtime/100).arg(simtime%100/10));
        drawArea->update();
    }
}

void Window::startSim(){
    drawArea->reset();
    drawAreaLabel->setText(tr("Simulating"));
    simtime=0;
    sim = new Simulation(interval);

    for(int i = 0; i < ptsList->count(); i++){
        Point p = ptsList->item(i)->data(Qt::UserRole).value<Point>();
        sim->addPoint(p);
    }
    timer->start(interval);

    start->setText(tr("Pause"));
    disconnect(start, SIGNAL(clicked()), this, SLOT(startSim()));
    connect(start, SIGNAL(clicked()), this, SLOT(pauseSim()));
    stop->setEnabled(true);
    restart->setEnabled(true);
}

void Window::stopSim(){
    drawAreaLabel->setText(tr("Ready"));
    timer->stop();
    delete sim;
    simtime=0;
    start->setText(tr("Start"));
    disconnect(start, SIGNAL(clicked()), this, SLOT(pauseSim()));
    connect(start, SIGNAL(clicked()), this, SLOT(startSim()));
    stop->setEnabled(false);
    restart->setEnabled(false);
}
void Window::restartSim(){
    timer->stop();
    simtime=0;
    delete sim;
    drawArea->reset();
    sim = new Simulation(interval);

    for(int i = 0; i < ptsList->count(); i++){
        Point p = ptsList->item(i)->data(Qt::UserRole).value<Point>();
        sim->addPoint(p);
    }
    timer->start(interval);
}

void Window::pauseSim(){
    if (timer->isActive() == true){
        timer->stop();
        start->setText(tr("Start"));
    }
    else{
        timer->start();
        start->setText(tr("Pause"));
    }
};

Window::~Window(){
    
}
