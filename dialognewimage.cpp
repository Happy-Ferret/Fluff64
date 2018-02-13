#include "dialognewimage.h"
#include "ui_dialognewimage.h"

DialogNewImage::DialogNewImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewImage)
{
    ui->setupUi(this);
}

DialogNewImage::~DialogNewImage()
{
    delete ui;
}
