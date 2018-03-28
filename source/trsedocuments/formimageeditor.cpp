#include "FormImageEditor.h"
#include "ui_FormImageEditor.h"
#include <algorithm>
#include <cmath>
#include <QPushButton>
#include "source/limage/charsetimage.h"


FormImageEditor::FormImageEditor(QWidget *parent) :
    TRSEDocument(parent),
    ui(new Ui::Formimageeditor)
{
    ui->setupUi(this);

    m_toolBox.Initialize(ui->lyToolbox_3);;
    UpdatePalette();
    m_grid.Initialize(320*2,200*2);
    m_grid.CreateGrid(40,25,m_gridColor,4, 1, QPoint(0,0));
    m_fileExtension = "flf";


    m_grid.ApplyToLabel(ui->lblGrid);
   // updateCharSet();
}

FormImageEditor::~FormImageEditor()
{
    delete ui;
}

void FormImageEditor::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::LeftButton) {

        m_work.m_currentImage->AddUndo();
    }
    if(e->buttons() == Qt::RightButton)
        m_updateThread->m_currentButton = 2;
    if(e->buttons() == Qt::LeftButton) {
        m_updateThread->m_currentButton = 1;
    }

}

void FormImageEditor::mouseReleaseEvent(QMouseEvent *e)
{
    if (m_updateThread->m_currentButton==2)
        m_updateThread->m_currentButton = 0;
    else
        m_updateThread->m_currentButton = -1;
}

void FormImageEditor::wheelEvent(QWheelEvent *event)
{
    float f = event->delta()/100.0f;

    if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
        m_updateThread->m_zoom -=f*0.05;
        m_updateThread->m_zoom = std::min(m_updateThread->m_zoom, 1.0f);
        m_updateThread->m_zoom = std::max(m_updateThread->m_zoom, 0.1f);
        float t = 0.0f;
        m_updateThread->m_zoomCenter = (m_updateThread->m_zoomCenter*t + (1-t)*m_updateThread->m_currentPos);//*(2-2*m_zoom);
        Data::data.redrawOutput = true;

        m_grid.CreateGrid(40,25,m_gridColor,4, m_updateThread->m_zoom, m_updateThread->m_zoomCenter);

    }
    else {
        m_toolBox.m_current->m_size +=f;
        m_toolBox.m_current->m_size = std::max(m_toolBox.m_current->m_size,1.0f);
        m_toolBox.m_current->m_size = std::min(m_toolBox.m_current->m_size,50.0f);
    }
    Data::data.redrawOutput = true;
    Data::data.forceRedraw = true;
}

void FormImageEditor::keyPressEvent(QKeyEvent *e)
{
    //    qDebug() << (QApplication::keyboardModifiers() & Qt::ShiftModifier) << " + "  << rand()%100;
        if (e->key()==Qt::Key_Shift) {
            m_toolBox.m_current->m_type = 1;
            Data::data.redrawOutput = true;
            Data::data.forceRedraw = true;
        }

        if (!ui->tblData->hasFocus() && !(QApplication::keyboardModifiers() & Qt::ControlModifier)) {
            m_work.m_currentImage->m_image->StoreData(ui->tblData);
            m_work.m_currentImage->m_image->KeyPress(e);
            m_work.m_currentImage->m_image->BuildData(ui->tblData, m_projectIniFile->getStringList("data_header"));
        }

        if (e->key()==Qt::Key_S && (QApplication::keyboardModifiers() & Qt::ControlModifier))
            SaveCurrent();

        FillCMBColors();

        updateCharSet();

        if (e->key()==Qt::Key_W && (QApplication::keyboardModifiers() & Qt::ControlModifier))
            Data::data.requestCloseWindow = true;

        if (e->key()==Qt::Key_Z && QApplication::keyboardModifiers() & Qt::ControlModifier) {
            m_work.m_currentImage->Undo();

        }
        if (e->key() == Qt::Key_Z  && !(QApplication::keyboardModifiers() & Qt::ControlModifier)) {
                ui->chkGrid->setChecked(!ui->chkGrid->isChecked());
                ui->lblGrid->setVisible(ui->chkGrid->isChecked());

        }
        if (e->key() == Qt::Key_X) {
            m_work.m_currentImage->m_image->renderPathGrid =!m_work.m_currentImage->m_image->renderPathGrid;
        }

        Data::data.forceRedraw = true;
        Data::data.Redraw();

}

void FormImageEditor::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key()==Qt::Key_Shift) {
        m_toolBox.m_current->m_type = 0;
        Data::data.redrawOutput = true;
        Data::data.forceRedraw = true;
    }

}

void FormImageEditor::UpdateImage()
{

//    m_updateThread->m_

    ui->lblImage->setPixmap(m_updateThread->m_pixMapImage);



    if (!ui->tblData->hasFocus())
        ui->lblImage->setFocus();

    ui->lblPosition->setText("Position: " +
                             QString::number(m_updateThread->m_currentPosInImage.x()) + ", " +
                             QString::number(m_updateThread->m_currentPosInImage.y()));


    m_grid.ApplyToLabel(ui->lblGrid);
    if (Data::data.redrawFileList) {
        //m_work.UpdateListView(ui->lstImages);
        Data::data.redrawFileList = false;
    }

}

void FormImageEditor::Load(QString filename)
{
/*    QString f = "Image Files (*." + LImageIO::m_fileExtension + ")";
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Image"), m_iniFile->getString("project_path"), f);
    if (filename=="")
        return;
*/
    LImage* img = LImageIO::Load(filename);

    m_work.New(img, filename);

    img->LoadCharset(m_iniFile->getString("current_charset"));
    updateCharSet();

    Data::data.redrawFileList = true;
    Data::data.Redraw();
    UpdatePalette();
    FillCMBColors();

    m_work.m_currentImage->m_image->BuildData(ui->tblData, m_projectIniFile->getStringList("data_header"));


}

void FormImageEditor::Save(QString filename)
{
    LImageIO::Save(filename,m_work.m_currentImage->m_image);

}

void FormImageEditor::SelectCharacter(int idx)
{
    m_work.m_currentImage->m_image->SetCurrentType(LImage::WriteType::Character);
//    Data::data.currentColor = idx;
    m_work.m_currentImage->m_image->setCurrentChar(idx);

}

Ui::Formimageeditor *FormImageEditor::getUi() const
{
    return ui;
}

QLabel* FormImageEditor::getLabelImage()
{
    return ui->lblImage;
}
void FormImageEditor::UpdatePalette()
{
    if (m_work.m_currentImage==nullptr)
        return;
    LColorList* l = &m_work.m_currentImage->m_image->m_colorList;
    //if (m_currentColorList!=l)
    //{

        l->CreateUI(ui->layoutColorsEdit_3,1);
        l->FillComboBox(ui->cmbBackgroundMain_3);
        l->FillComboBox(ui->cmbBorderMain_3);
        l->FillComboBox(ui->cmbMC1);
        l->FillComboBox(ui->cmbMC2);
        m_currentColorList = l;
    //}

    if (m_work.m_currentImage==nullptr)
        return;

    if (m_work.m_currentImage->m_image==nullptr)
        return;

    ui->cmbMC1->setCurrentIndex(m_work.m_currentImage->m_image->m_extraCols[1]);
    ui->cmbMC2->setCurrentIndex(m_work.m_currentImage->m_image->m_extraCols[2]);

    ui->btnExportBin->setVisible(m_work.m_currentImage->m_image->m_supports.binarySave);
    ui->btnImportBin->setVisible(m_work.m_currentImage->m_image->m_supports.binaryLoad);
    ui->btnLoad->setVisible(m_work.m_currentImage->m_image->m_supports.flfLoad);
    ui->btnSave->setVisible(m_work.m_currentImage->m_image->m_supports.flfSave);
    ui->btnExportAsm->setVisible(m_work.m_currentImage->m_image->m_supports.asmExport);

}

void FormImageEditor::FillCMBColors()
{
    ui->cmbBackgroundMain_3->setCurrentIndex(m_work.m_currentImage->m_image->m_background);
    ui->cmbMC1->setCurrentIndex(m_work.m_currentImage->m_image->m_extraCols[1]);
    ui->cmbMC2->setCurrentIndex(m_work.m_currentImage->m_image->m_extraCols[2]);

}

void FormImageEditor::resizeEvent(QResizeEvent *event)
{
    ui->lblImage->setVisible(true);
    ui->lblGrid->setGeometry(ui->lblImage->geometry());
    ui->lblGrid->repaint();
   // qDebug() <<
//    qDebug() << ui->lblImage->geometry();
  //  ui->lblImage->setVisible(false);
}


void FormImageEditor::on_btnExportAsm_clicked()
{

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export Multicolor Assembler image"), m_projectIniFile->getString("project_path"),
                                                    tr("Bin (*.bin);"));


//    m_work.m_currentImage->m_image->ExportAsm(fileName);
    MultiColorImage* mi = (MultiColorImage*)dynamic_cast<MultiColorImage*>(m_work.m_currentImage->m_image);

    if (mi==nullptr)
        return;

    //fileName.remove(".bin");

    mi->ExportAsm(fileName);
//    mi->ExportRasBin(fileName, "");
}


/*void FormImageEditor::on_btnSave_clicked()
{
    QString filename = m_work.m_currentImage->m_fileName;

    if (filename=="")
        filename = QFileDialog::getSaveFileName(this,
                                                tr("Save Image"), m_iniFile->getString("project_path"), ("Image Files (*." + LImageIO::m_fileExtension + ")"));
    if (filename=="")
        return;

    LImageIO::Save(filename,m_work.m_currentImage->m_image);

}
*/
void FormImageEditor::on_chkGrid_clicked(bool checked)
{
//    if (checked)
        ui->lblGrid->setVisible(checked);
}




void FormImageEditor::on_btnNew_clicked()
{
    m_work.m_currentImage->m_image->Clear();
    Data::data.Redraw();
}

void FormImageEditor::on_btnExportImage_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Image"), m_iniFile->getString("project_path"), tr("Image Files (*.png *.jpg *.bmp )"));

    if (fileName == "")
        return;

    m_updateThread->m_tmpImage->save(fileName);


}

void FormImageEditor::on_b_clicked()
{
}

void FormImageEditor::on_lstImages_clicked(const QModelIndex &index)
{
    m_work.SetImage(index.row());
    Data::data.redrawFileList = true;
    UpdatePalette();
}

void FormImageEditor::on_btnImport_clicked()
{
    DialogImport* di = new DialogImport(this);
    di->Initialize(m_work.m_currentImage->m_imageType->type, m_work.m_currentImage->m_image->m_colorList.m_type);
    di->exec();
    if (di->m_ok) {
        m_work.m_currentImage->m_image->CopyFrom(di->m_image);
        Data::data.redrawOutput = true;
    }

}

void FormImageEditor::on_btnCharsetFull_clicked()
{
    CharsetImage* ci = dynamic_cast<CharsetImage*>(m_work.m_currentImage->m_image);
    if (ci==nullptr)
        return;

    ci->m_currentMode = CharsetImage::Mode::FULL_IMAGE;
}

void FormImageEditor::on_btnCharset1x1_clicked()
{
    CharsetImage* ci = dynamic_cast<CharsetImage*>(m_work.m_currentImage->m_image);
    if (ci==nullptr)
        return;

    ci->m_currentMode = CharsetImage::Mode::CHARSET1x1;

}

void FormImageEditor::on_btnCharset2x2_clicked()
{
    CharsetImage* ci = dynamic_cast<CharsetImage*>(m_work.m_currentImage->m_image);
    if (ci==nullptr)
        return;

    ci->m_currentMode = CharsetImage::Mode::CHARSET2x2;

}

void FormImageEditor::on_btnCharset2x2Repeat_clicked()
{
    CharsetImage* ci = dynamic_cast<CharsetImage*>(m_work.m_currentImage->m_image);
    if (ci==nullptr)
        return;

    ci->m_currentMode = CharsetImage::Mode::CHARSET2x2_REPEAT;

}

void FormImageEditor::on_btnCharsetCopy_clicked()
{
    m_work.m_currentImage->m_image->CopyChar();
}

void FormImageEditor::on_btnCharsetPaste_clicked()
{
    m_work.m_currentImage->m_image->PasteChar();

}

void FormImageEditor::on_btnTiff_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Tiled Tiff"), m_projectIniFile->getString("project_path"), tr("Image Files (*.tif *.tiff )"));

    if (fileName == "")
        return;


    LImageTiff *tif = (LImageTiff*)LImageFactory::Create(LImage::Tiff, LColorList::TIFF);
    tif->Initialize(640,480);
    tif->LoadTiff(fileName);
//    tif->m_type = LImage::Tiff;
    m_work.New(tif, fileName);
    Data::data.redrawFileList = true;
    Data::data.Redraw();
    UpdatePalette();

}

void FormImageEditor::on_btnSaveAs_clicked()
{

    QString filename = QFileDialog::getSaveFileName(this,
                                                tr("Save Image"), m_iniFile->getString("project_path"), ("Image Files (*." + LImageIO::m_fileExtension + ")"));
    if (filename=="")
        return;
    m_work.m_currentImage->m_fileName  =filename;

    LImageIO::Save(filename,m_work.m_currentImage->m_image);

}




void FormImageEditor::updateCharSet()
{
    CharsetImage* charmap = m_work.m_currentImage->m_image->getCharset();
    if (charmap == nullptr)
        return;

    QVector<QPixmap> maps;
    charmap->ToQPixMaps(maps);

  /*  Util::clearLayout(ui->grdChars);
    int i=0;
    int j=0;
    for (int cnt=0;cnt<maps.count();cnt++) {
        QPushButton* item = new QPushButton(this);
        item->setAutoFillBackground(true);
        item->setText("");
        item->setIcon(maps[cnt]);
        QObject::connect( item, &QPushButton::clicked,  [=](){ SelectCharacter(cnt);} );

        ui->grdChars->addWidget(item, j,i);

        i++;
        if (i>=8) {
            i=0;
            j++;
        }
    }
*/
//   ui->lstCharMap->setViewMode(QListView::IconMode);
   ui->lstCharMap->setColumnCount(40);
   ui->lstCharMap->setRowCount(maps.count()/40);
    int cnt=0;
    int j=0;
    int i=0;
    int size=32;
    ui->lstCharMap->setIconSize(QSize(size,size));
    for (int i=0;i<40;i++) {
        ui->lstCharMap->setColumnWidth(i,size);
        //ui->lstCharMap->setCol
    }
    QStringList lst;
    for (int i=0;i<40;i++)
         lst<<"";
    ui->lstCharMap->setHorizontalHeaderLabels(lst);

    int kk= 0;
    for (int k=0;k<maps.count();k++) {

        QPixmap q = maps[k];

        QTableWidgetItem *itm = ui->lstCharMap->item(j,i);
        if (itm == nullptr) {
            itm = new QTableWidgetItem(q,nullptr,Qt::DisplayRole);
            ui->lstCharMap->setItem(j,i,itm);

        }
        itm->setIcon(q);
        itm->setData(Qt::UserRole, kk);
        cnt++;
        i++;
        kk++;
        if (i>=40) {
            i=0;
            j++;
           // kk+=40-8;

        }

       // kk=kk%255;

    }
//    ui->lstCharMap->
//    int size = (ui->lstCharMap->rect().width()-ui->lstCharMap->spacing())/8;
  //  ui->lstCharMap->setIconSize(QSize(size,size));

}

void FormImageEditor::SetMCColors()
{
    int a = ui->cmbMC1->currentIndex();
    int b = ui->cmbMC2->currentIndex();
    int back = ui->cmbBackgroundMain_3->currentIndex();

    m_work.m_currentImage->m_image->SetColor(back, 0);
    m_work.m_currentImage->m_image->SetColor(a, 1);
    m_work.m_currentImage->m_image->SetColor(b, 2);

    updateCharSet();
}

void FormImageEditor::UpdateLevels()
{
    ImageLevelEditor* le = dynamic_cast<ImageLevelEditor*>(m_work.m_currentImage->m_image);
    if (le==nullptr)
        return;

    QVector<QPixmap> icons = le->CreateIcons();


    Util::clearLayout(ui->gridLevels);
//    ui->gridLevels->set
    for (int i=0;i<le->m_meta.m_sizex;i++)
        for (int j=0;j<le->m_meta.m_sizey;j++) {
            QPushButton* l = new QPushButton();
            l->setText("");
            QIcon icon(icons[j + i*le->m_meta.m_sizey]);
            l->setIcon(icon);
            //l->setScaledContents(true);
            l->setIconSize(QSize(64,64));
            ui->gridLevels->addWidget(l, j,i);

            QObject::connect( l, &QPushButton::clicked,  [=](){
                m_work.m_currentImage->m_image->StoreData(ui->tblData);
                le->SetLevel(QPoint(i,j));
                m_work.m_currentImage->m_image->BuildData(ui->tblData,m_projectIniFile->getStringList("data_header"));
                Data::data.Redraw();
            }

            );

        }



}


void FormImageEditor::on_btnSave_2_clicked()
{

}



void FormImageEditor::on_btnExpChar_clicked()
{
/*    if (m_work.m_currentImage->m_imageType->type==LImage::Type::HiresBitmap ||
        m_work.m_currentImage->m_imageType->type==LImage::Type::MultiColorBitmap
            )
    {
        MultiColorImage* img = (MultiColorImage*)m_work.m_currentImage->m_image;
        img->CalculateCharIndices();
        qDebug() <<"Number of chars: " <<img->m_organized.count();
        QString filename = m_iniFile->getString("project_path") + "/testchar.inc";
        img->SaveCharRascal(filename,"test");
        RefreshFileList();
    }
*/

}

void FormImageEditor::on_btnImportBin_clicked()
{
    QString f = "Binary Files ( *.bin )";
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Import binary file"), m_projectIniFile->getString("project_path"), f);
    if (filename=="")
        return;


    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    m_work.m_currentImage->m_image->ImportBin(file);
    file.close();

    Data::data.redrawFileList = true;
    Data::data.Redraw();
    UpdatePalette();

}

void FormImageEditor::on_btnExportBin_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export binary file"), m_projectIniFile->getString("project_path"),
                                                    tr("Bin (*.bin);"));


//    m_work.m_currentImage->m_image->ExportAsm(fileName);
//    MultiColorImage* mi = (MultiColorImage*)dynamic_cast<MultiColorImage*>(m_work.m_currentImage->m_image);

    if (QFile::exists(fileName))
        QFile::remove(fileName);
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    m_work.m_currentImage->m_image->ExportBin(file);
    file.close();

  //  fileName.remove(".bin");

//    mi->ExportRasBin(fileName, "");

}

void FormImageEditor::on_tabWidget_2_currentChanged(int index)
{
    if (index==1)
        m_work.m_currentImage->m_image->SetCurrentType(LImage::WriteType::Color);
    if (index==3)
        UpdateLevels();
//    if (index==2)
//        m_work.m_currentImage->m_image->SetCurrentType(LImage::WriteType::Character);
}

void FormImageEditor::on_btnLoadCharmap_clicked()
{
    C64FullScreenChar* charImage = dynamic_cast<C64FullScreenChar*>(m_work.m_currentImage->m_image);

    ImageLevelEditor* editor = dynamic_cast<ImageLevelEditor*>(m_work.m_currentImage->m_image);

    if (editor==nullptr && charImage==nullptr)
        return;


    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Character map"), m_projectIniFile->getString("project_path"), tr("Binary Files (*.bin )"));

    if (fileName == "")
        return;


    editor->LoadCharset(fileName);
    m_iniFile->setString("current_charset", fileName);
    m_iniFile->Save();
    updateCharSet();

}

void FormImageEditor::on_lstCharMap_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{

    int idx = current->data(Qt::UserRole).toInt();
    m_work.m_currentImage->m_image->SetCurrentType(LImage::WriteType::Character);
   // Data::data.currentColor = idx;
    m_work.m_currentImage->m_image->setCurrentChar(idx);

    Data::data.Redraw();
    Data::data.forceRedraw = true;


}

void FormImageEditor::Destroy()
{
    //delete m_work.m_currentImage;
}


void FormImageEditor::on_cmbMC1_activated(int index)
{
    SetMCColors();

}

void FormImageEditor::on_cmbMC2_activated(int index)
{
    SetMCColors();

}

void FormImageEditor::on_cmbBackgroundMain_3_activated(int index)
{
    m_work.m_currentImage->m_image->setBackground(index);
    SetMCColors();
    Data::data.redrawOutput = true;

}

void FormImageEditor::on_btnResizeData_clicked()
{
    ImageLevelEditor* img = dynamic_cast<ImageLevelEditor*>(m_work.m_currentImage->m_image);
    if (img==nullptr)
        return;


    DialogNewImage* dResize = new DialogNewImage(this);
    dResize->Initialize(m_work.getImageTypes());
    dResize->setModal(true);
    dResize->SetResizeMeta(img->m_meta);

    dResize->exec();
    if (dResize->retVal!=-1) {
        img->Resize(dResize->m_meta);

    }



    UpdatePalette();

    delete dResize;


}
