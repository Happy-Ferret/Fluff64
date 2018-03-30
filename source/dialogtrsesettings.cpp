#include "dialogtrsesettings.h"
#include "ui_dialogtrsesettings.h"
#include <QFileDialog>
#include <QStringList>
#include <QDir>


DialogTRSESettings::DialogTRSESettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTRSESettings)
{
    ui->setupUi(this);
}

void DialogTRSESettings::FillFromIni()
{
    ui->leDasm->setText(m_ini->getString("dasm"));
    ui->leEmulator->setText(m_ini->getString("emulator"));

    QDir directory("themes/");
    QStringList themes = directory.entryList(QStringList() << "*.ini");
    ui->cmbTheme->clear();
    QStringList washed;
    for (QString s: themes) {
        QString name = s.split(".")[0];
      //  qDebug() << name;
        washed << name;
    }
    ui->cmbTheme->addItems(washed);
    ui->cmbTheme->setCurrentText(m_ini->getString("theme").split(".")[0]);

}

void DialogTRSESettings::FillToIni()
{
    m_ini->setString("dasm", ui->leDasm->text());
    m_ini->setString("emulator", ui->leEmulator->text());
    m_ini->setString("theme", ui->cmbTheme->currentText() + ".ini");
}

DialogTRSESettings::~DialogTRSESettings()
{
    delete ui;
}

void DialogTRSESettings::on_pushButton_clicked()
{
    FillToIni();
    m_ini->Save();
    close();
}

void DialogTRSESettings::on_btnEmulator_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Emulator location"), m_ini->getString("project_path"), "*.exe");
    ui->leEmulator->setText(filename);

}

void DialogTRSESettings::on_btnDasm_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Dasm location"), m_ini->getString("project_path"), "*.exe");
    ui->leDasm->setText(filename);

}