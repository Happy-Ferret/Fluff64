#include "lcolorlist.h"
#include <QDebug>
#include <QPalette>
#include <QSignalMapper>
LColorList::LColorList()
{

}

void LColorList::InitC64_org()
{
    m_list.clear();
    m_list.append(LColor(QColor(0,0,0),""));
    m_list.append(LColor(QColor(255,255,255),""));
    m_list.append(LColor(QColor(136,0,0),""));
    m_list.append(LColor(QColor(170,255,238),""));
    m_list.append(LColor(QColor(204,68,204),""));
    m_list.append(LColor(QColor(0,204,85),""));
    m_list.append(LColor(QColor(0,0,170),""));
    m_list.append(LColor(QColor(238,238,119),""));
    m_list.append(LColor(QColor(221,136,85),""));
    m_list.append(LColor(QColor(102,68,0),""));
    m_list.append(LColor(QColor(255,119,119),""));
    m_list.append(LColor(QColor(51,51,51),""));
    m_list.append(LColor(QColor(119,119,119),""));
    m_list.append(LColor(QColor(170,255,102),""));
    m_list.append(LColor(QColor(0,136,255),""));
    m_list.append(LColor(QColor(187,187,198),""));

    m_metric = new LinearMetric();
}

void LColorList::InitC64()
{
    m_list.clear();
    m_list.append(LColor(QColor(0x0, 0x0, 0x0),""));
    m_list.append(LColor(QColor(0xFF, 0xFF, 0xFF),""));
    m_list.append(LColor(QColor(0x68, 0x37, 0x2b),""));
    m_list.append(LColor(QColor(0x70, 0xa4, 0xb2),""));
    m_list.append(LColor(QColor(0x6f, 0x3d, 0x86),""));
    m_list.append(LColor(QColor(0x58, 0x8d, 0x43),""));
    m_list.append(LColor(QColor(0x35, 0x28, 0x79),""));
    m_list.append(LColor(QColor(0xb8, 0xc7, 0x6F),""));
    m_list.append(LColor(QColor(0x6f, 0x4f, 0x25),""));
    m_list.append(LColor(QColor(0x43, 0x39, 0x00),""));
    m_list.append(LColor(QColor(0x9a, 0x67, 0x59),""));
    m_list.append(LColor(QColor(0x44, 0x44, 0x44),""));
    m_list.append(LColor(QColor(0x6c, 0x6c, 0x6c),""));
    m_list.append(LColor(QColor(0x9a, 0xd2, 0x84),""));
    m_list.append(LColor(QColor(0x6c, 0x5e, 0xb5),""));
    m_list.append(LColor(QColor(0x95, 0x95, 0x95),""));

    m_background = m_list[0];

    m_metric = new LinearMetric();
}

QColor LColorList::getClosestColor(QColor col)
{
    float d = 1E20;
    int winner=0;
    int i = 0;
//    qDebug() << "WHOO";
    for (int i=0;i<m_list.count();i++) {
        //qDebug() << "Metric:";
        if (!m_list[i].inUse)
            continue;
        float v = m_metric->getDistance(m_list[i].color, col);
        //qDebug() << "end:";
        if (v<d) {
            d = v;
            winner = i;
        }
    }
/*    if (rand()%500==0) {
        qDebug() << "Testing for: " << col.red() << ", " << col.green() << ", " << col.blue();
        qDebug() << "Winner: " <<m_list[winner].color.red() << ", " <<m_list[winner].color.green() << ", " << m_list[winner].color.blue();
    }*/
    return m_list[winner].color;

}

void LColorList::FillComboBox(QComboBox *cmb)
{
    cmb->clear();
    for (int i=0;i<m_list.count();i++) {
        QPixmap pixmap(16,16);
        pixmap.fill(m_list[i].color);
        QIcon icon(pixmap);
        cmb->addItem(icon,"");
    }
}

int LColorList::getIndex(QColor c)
{
    for (int i=0;i<m_list.count();i++) {
        if (m_list[i].color == c)
            return i;
    }
    return 0;
}

void LColorList::CreateUI(QLayout* ly, int type)
{
//    m_buttons.clear();
    for(int j=0; j<m_list.count(); j++)
    {
        QPushButton *b = new QPushButton();
        //b->setGeometry(0,0,40,40);
        QPalette p;
        p.setColor(QPalette::Button, m_list[j].color);
        p.setColor(QPalette::Window, m_list[j].color);
        QString txtCol = QString::number(m_list[j].color.red()) + ", ";
        txtCol += QString::number(m_list[j].color.green()) + ", ";
        txtCol += QString::number(m_list[j].color.blue());
        b->setStyleSheet("background-color: rgb("+txtCol + "); color: rgb(0, 0, 0)");
        b->setPalette(p);
        b->setMaximumWidth(40);
        b->setMinimumWidth(40);
        b->setAutoFillBackground( true );
        if (type==0) {
            QObject::connect( b, &QPushButton::clicked,  [=](){ handleButtonImport(j);} );
        }
        if (type==1) {
            QObject::connect( b, &QPushButton::clicked,  [=](){ handleButtonEdit(j); } );
        }
        //QObject::connect( b, &QPushButton::clicked,  colorValueChanged );


        ly->addWidget(b);
        if (type==0)
            m_buttonsImport.append(b);
    }
}

void LColorList::handleButtonEdit(int data)
{
    Data::data.currentColor = data;

}

void LColorList::handleButtonImport(int data)
 {
    //qDebug() << data;
    m_list[data].inUse=!m_list[data].inUse;
    if (m_list[data].inUse)
        m_buttonsImport[data]->setText(" ");
    else {
        m_buttonsImport[data]->setText("X ");
   }

    //Data::data.redrawInput = true;
    emit colorValueChanged();
 }
