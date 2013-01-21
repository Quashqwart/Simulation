#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <widget.h>
#include <QLabel>
#include <simulation.h>
#include <QTimer>
#include <QPushButton>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QListWidgetItem>
#include <QListWidget>
#include <QPalette>
#include <vector>

class Window : public QWidget
{
    Q_OBJECT
    
public:
    Window();
    ~Window();

private:
    int interval, redraw_interval, simtime;
    Widget *drawArea;
    QLabel *drawAreaLabel;
    Simulation *sim;
    QTimer *timer;
    QSpinBox *px, *py, *pvx, *pvy, *pmass, *ngen;
    QLabel *cLabel, *vLabel, *massLabel, *colorLabel;
    QPushButton *start, *stop, *clrBtn, *restart;
    QLineEdit *filename;
    QListWidget *ptsList;
    QPalette *pal;
private slots:
    void step();
    void startSim();
    void pauseSim();
    void restartSim();
    void stopSim();
    void addPoint();
    void deletePoint();
    void loadConf();
    void editPoint();
    void selectColor();
    void saveConf();
    void genConf();
    void itemClicked(QModelIndex);
};

#endif // WINDOW_H
