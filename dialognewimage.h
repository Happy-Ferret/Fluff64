#ifndef DIALOGNEWIMAGE_H
#define DIALOGNEWIMAGE_H

#include <QDialog>
#include <QDebug>
namespace Ui {
class DialogNewImage;
}

class DialogNewImage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewImage(QWidget *parent = 0);
    void Initialize(QStringList cmbData);
    int retVal = -1;
    ~DialogNewImage();
private slots:
    void slotOk();

private:
    Ui::DialogNewImage *ui;
};

#endif // DIALOGNEWIMAGE_H
