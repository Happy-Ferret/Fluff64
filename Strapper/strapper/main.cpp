#include <QProcess>
#include <iostream>
#include <QStringlist>
#include "updater.h"
using namespace std;

float currentVersion = 0.001;

void CheckVersion() {
    Updater u;
    u.Initialize();
    if (!u.CheckVersionOK(currentVersion)) {
        qDebug() << "New version of Turbo Rascal Syntax Error (" + QString::number(u.m_newVersion) +") released, downloading...";
        u.FetchNewVersion();

        QProcess::startDetached("bin/7z.exe", QStringList() << "x"<<"trse_win.zip" <<"-aoa" << "-O..\\", "." );



    }
    else qDebug() << "No new version. Continuing.";
}


int main(int argc, char *argv[])
{
    cout << "This is TRSE updater currently version " << QString::number(currentVersion).toStdString() << endl;
    CheckVersion();
    QProcess::startDetached("bin/trse.exe",QStringList(),"bin/");
}
