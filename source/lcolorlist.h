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
#include <QLayout>

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

class LColorList : public QObject
{
    Q_OBJECT

public:
    enum Type{ C64, C64_ORG, CGA1_LOW, CGA1_HIGH, CGA2_LOW, CGA2_HIGH };

    Type m_type = Type::C64;
    LColorList();
    ~LColorList();

    QVector<LColor> m_list;
    QVector<QPushButton*> m_buttonsImport;
    QVector<QPushButton*> m_buttonsEdit;

    Metric* m_metric = nullptr;

    void Initialize(Type t);

    void InitC64_org();
    void InitC64();
    void InitCGA1_LOW();
    void InitCGA1_HIGH();
    void InitCGA2_LOW();
    void InitCGA2_HIGH();
    void UpdateColors();
    QColor getClosestColor(QColor col);

    void FillComboBox(QComboBox* cmb);

    LColor m_background;

    int getIndex(QColor c);

    void handleButtonEdit(int data);

    void CreateUI(QLayout* ly, int type);

public slots:
    void handleButtonImport(int data);


signals:
    void colorValueChanged();

};

#endif // LCOLORLIST_H
