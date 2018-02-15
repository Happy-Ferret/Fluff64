#include "dialognewimage.h"
#include "ui_dialognewimage.h"

DialogNewImage::DialogNewImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewImage )
{
    ui->setupUi(this);
    connect(ui->btnResult, SIGNAL(accepted()), this, SLOT(slotOk()));

}

void DialogNewImage::Initialize(QStringList cmbData)
{
    ui->comboBox->addItems(cmbData);
}

DialogNewImage::~DialogNewImage()
{
    delete ui;
}

void DialogNewImage::slotOk()
{
    retVal = ui->comboBox->currentIndex();
}
