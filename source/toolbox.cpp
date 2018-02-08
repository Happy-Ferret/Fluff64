#include "toolbox.h"


Toolbox::Toolbox()
{

}

Toolbox::ToolBox()
{
}

void Toolbox::Initialize(QLayout* ly)
{
    m_items.clear();
    m_items.append(new ShapeBox("Rectangle",""));

    m_current = m_items[0];

    BuildGUI(ly);

}

void Toolbox::BuildGUI(QLayout *ly)
{
    for (int i=0;i<m_items.count();i++) {
        QPushButton *b = new QPushButton();
        //b->setGeometry(0,0,40,40);
/*        QPalette p;
        p.setColor(QPalette::Button, m_list[j].color);
        p.setColor(QPalette::Window, m_list[j].color);*/
//        b->setStyleSheet("background-color: rgb("+txtCol + "); color: rgb(0, 0, 0)");
  //      b->setPalette(p);
        b->setMaximumWidth(40);
        b->setMinimumWidth(40);
        b->setAutoFillBackground( true );
        b->setText(m_items[i]->m_name);
        QObject::connect( b, &QPushButton::clicked,  [=](){ handleButton(i); } );

        ly->addWidget(b);
        m_buttons.append(b);

    }
}

void Toolbox::handleButton(int data)
{
    m_current = m_items[data];
}
