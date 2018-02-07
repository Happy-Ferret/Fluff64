#ifndef LCOLORLIST_H
#define LCOLORLIST_H

#include <QColor>
#include <QVector>
#include <QString>
#include <QComboBox>
#include "ui_mainwindow.h"
#include "data.h"
#include <QPushButton>
#include <QIcon>
#include <QPixmap>
class Metric {
public:
    virtual float getDistance(QColor& a, QColor& b) = 0;
};

class LinearMetric: public Metric {
public:
    float getDistance(QColor& a, QColor& b) override {
        return sqrt(1+ pow(a.red() - b.red(), 2) +pow(a.green() - b.green(), 2) +pow(a.blue() - b.blue(), 2)  );
    }
};


class LColor {
public:
    QColor color;
    bool inUse = true;
    QString name;
    LColor() {}
    LColor(QColor col, QString n) {
        color = col;
        name = n;
    }

};

class LColorList
{
public:
    LColorList();

    QVector<LColor> m_list;
    QVector<QPushButton*> m_buttons;

    Metric* m_metric = nullptr;
    void InitC64_org();
    void InitC64();
    void UpdateColors();
    QColor getClosestColor(QColor col);

    void FillComboBox(QComboBox* cmb);

    LColor m_background;

    int getIndex(QColor c);

    void CreateUI(Ui::MainWindow* ui);
    void handleButton(int data);

};

#endif // LCOLORLIST_H
