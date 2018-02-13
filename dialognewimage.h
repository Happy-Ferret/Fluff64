#ifndef DIALOGNEWIMAGE_H
#define DIALOGNEWIMAGE_H

#include <QDialog>

namespace Ui {
class DialogNewImage;
}

class DialogNewImage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewImage(QWidget *parent = 0);
    ~DialogNewImage();

private:
    Ui::DialogNewImage *ui;
};

#endif // DIALOGNEWIMAGE_H
