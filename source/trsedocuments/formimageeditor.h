#ifndef FormImageEditor_H
#define FormImageEditor_H

#include <QWidget>
#include "trsedocument.h"
#include "source/toolbox.h"
#include "source/limage/limage.h"
#include "source/limage/limageqimage.h"
#include "imageworker.h"
#include <QListWidget>
#include <QListWidgetItem>
#include "source/limage/limageio.h"
#include "dialognewimage.h"
#include "dialogimport.h"
#include "source/util/util.h"

namespace Ui {
class Formimageeditor;
}

class FormImageEditor :  public TRSEDocument
{
    Q_OBJECT

public:
    explicit FormImageEditor(QWidget *parent = 0);
    ~FormImageEditor();
    ImageWorker m_work;

    LImageQImage m_grid;
    Toolbox m_toolBox;
    QColor m_gridColor = QColor(64,128,128,128);
    LColorList* m_currentColorList = nullptr;


    void UpdatePalette();
    void updateCharSet();

    void SetMCColors();
    void UpdateLevels();
    void FillCMBColors();

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void UpdateImage();

    void Load(QString filename) override;
    void Save(QString filename) override;


    Ui::Formimageeditor *getUi() const;

    QLabel* getLabelImage();




private:
    Ui::Formimageeditor *ui;

private slots:
    void on_btnExportAsm_clicked();


    void SelectCharacter(int j);


//    void on_btnLoad_clicked();

 //   void on_btnSave_clicked();

    void on_chkGrid_clicked(bool checked);


    void on_btnNew_clicked();

    void on_btnExportImage_clicked();

    void on_b_clicked();

    void on_lstImages_clicked(const QModelIndex &index);

    void on_btnImport_clicked();

    void on_btnTiff_clicked();

    void on_btnSaveAs_clicked();

    void on_btnSave_2_clicked();





    void on_btnExpChar_clicked();


    void on_btnImportBin_clicked();

    void on_btnExportBin_clicked();

    void on_tabWidget_2_currentChanged(int index);

    void on_btnLoadCharmap_clicked();

    void on_lstCharMap_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

    void Destroy();

    void on_cmbMC1_activated(int index);

    void on_cmbMC2_activated(int index);

    void on_cmbBackgroundMain_3_activated(int index);



    void on_btnResizeData_clicked();

};

#endif // FormImageEditor_H
