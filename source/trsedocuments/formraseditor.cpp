#include "formraseditor.h"
#include "ui_formraseditor.h"
#include <QProcess>
#include "source/errorhandler.h"
#include <QElapsedTimer>
#include "source/util/util.h"


FormRasEditor::FormRasEditor(QWidget *parent) :
    TRSEDocument(parent),
    ui(new Ui::FormRasEditor)
{
    ui->setupUi(this);
    m_fileExtension = "ras";
}

FormRasEditor::~FormRasEditor()
{
    delete ui;
}

void FormRasEditor::ExecutePrg(QString fileName, QString emulator)
{
    if (!QFile::exists(emulator)) {
        Messages::messages.DisplayMessage(Messages::messages.NO_EMULATOR);
        return;
    }
    QProcess process;
    process.waitForFinished();
#ifdef _WIN32
    QProcess::execute("taskkill /im \"x64.exe\" /f");
#endif
    process.startDetached(emulator, QStringList() << fileName);
//    process.pi
    QString output(process.readAllStandardOutput());

}

void FormRasEditor::InitDocument(WorkerThread *t, CIniFile *ini, CIniFile* pro)
{
    TRSEDocument::InitDocument(t,ini, pro);
    FillFromIni();
    setupEditor();
}



void FormRasEditor::setupEditor()
{
    m_font.setFamily("Courier");
    m_font.setFixedPitch(true);
    m_font.setPointSize(m_iniFile->getdouble("font_size"));
    //ui->txtEditor->setTextColor(QColor(220,210,190));
    SetupHighlighter();
//    highlighter->Save("dark_standard.ini");

    UpdateFromIni();
//    ui->txtEditor->setTabStopWidth(m_iniFile->getInt("tab_width") * metrics.width(' '));

}

void FormRasEditor::Compress()
{

}

void FormRasEditor::Build()
{
    SaveCurrent();

    if (BuildStep())
        {
        interpreter.SaveBuild(filename + ".asm");
        QString text ="Build <b><font color=\"#90FF90\">Successful</font>!</b> ( "+  (Util::MilisecondToString(timer.elapsed())) +")<br>";
        text+="Assembler file saved to : <b>" + filename+".asm</b><br>";
        text+="Compiled <b>" + QString::number(parser.m_lexer->m_lines.count()) +"</b> of Rascal to <b>";
        text+=QString::number(interpreter.m_assembler->getLineCount()) + "</b> lines of DASM assembler instructions (and variables/labels)<br>";
        text+="Post-optimized away <b>" + QString::number(interpreter.m_assembler->m_totalOptimizedLines) +"</b> lines of assembler instructions<br>";
//        text+="+"<br>";
        text+="**** DASM output:<br>";

        QProcess process;
        process.start(m_iniFile->getString("dasm"), QStringList()<<(filename +".asm") << ("-o"+filename+".prg"));
        process.waitForFinished();
        //process;
        QProcess processCompress;
        if (m_iniFile->getdouble("perform_crunch")==1) {
            QString fn = (filename +".prg");
            if (!QFile::exists(m_iniFile->getString("exomizer")))
                Messages::messages.DisplayMessage(Messages::messages.NO_EXOMIZER);
            processCompress.start(m_iniFile->getString("exomizer"), QStringList()<< "sfx" << "$0900" << fn<< "-o" << fn  );
            processCompress.waitForFinished();
        }
        QString output(process.readAllStandardOutput());
        QString size = QString::number(QFile(filename+".prg").size());


        m_buildSuccess = true;

        if (output.toLower().contains("error")) {
            m_buildSuccess = false;
            if (output.toLower().contains("branch out of range")) {
                Messages::messages.DisplayMessage(Messages::messages.BRANCH_ERROR);
                output += "<br>Please check your <b>onpage/offpage</b> keywords.";

            }
            else
            if (output.toLower().contains("reverse-indexed")) {
                Messages::messages.DisplayMessage(Messages::messages.MEMORY_OVERLAP_ERROR);
                output += "<br>Please reorganize your binary inclusions in ascending order of memory locations.";
            }

            else
                Messages::messages.DisplayMessage(Messages::messages.DASM_COMPILER_ERROR);

        }
        if (!output.toLower().contains("complete.")) {
            m_buildSuccess = false;
            if (output=="") {
                Messages::messages.DisplayMessage(Messages::messages.NO_DASM);

                output = output + "\nCould not find Dasm.exe. Did you set the correct environment variables?";
            }

        }



        if (m_buildSuccess)
            output +="<br>Assembled file size: <b>" + size + "</b> bytes";

        //ui->ed
        ui->txtOutput->setText(text + output);
        ui->txtEditor->m_cycles =  interpreter.m_assembler->m_cycles;
        ui->txtEditor->RepaintCycles();
        ui->txtEditor->InitCompleter(interpreter.m_assembler->m_symTab, &parser);

    }
    else {
        ui->txtOutput->setText(ErrorHandler::e.m_teOut);
        int ln = Pmm::Data::d.lineNumber;
        QTextCursor cursor(ui->txtEditor->document()->findBlockByLineNumber(ln-1));
        ui->txtEditor->setTextCursor(cursor);
        m_buildSuccess = false;

    }
    SetLights();
}

void FormRasEditor::Setup()
{
      setupEditor();
}

void FormRasEditor::Run()
{
    if (!m_buildSuccess)
        return;
    QString filename = m_currentSourceFile.split(".")[0] + ".prg";

    ExecutePrg(filename,m_iniFile->getString("emulator"));

}

void FormRasEditor::SetLights()
{
    if (!m_buildSuccess)
        ui->lblLight->setStyleSheet("QLabel { background-color : red; color : blue; }");
    else
        ui->lblLight->setStyleSheet("QLabel { background-color : green; color : blue; }");


}

void FormRasEditor::SetText(QString s)
{
    ui->txtEditor->setPlainText(s);
}

void FormRasEditor::SetupHighlighter()
{
    if (highlighter != nullptr)
        delete highlighter;
    CIniFile colors;
    colors.Load("themes/" + m_iniFile->getString("theme"));
    QPalette p = ui->txtEditor->palette();
    p.setColor(QPalette::Base, colors.getColor("backgroundcolor"));
    p.setColor(QPalette::Text, colors.getColor("textcolor"));
    ui->txtEditor->setPalette(p);
    highlighter = new Highlighter(colors, ui->txtEditor->document());

//    qDebug() << "UPDATE " << m_iniFile->getString("theme");

}

void FormRasEditor::wheelEvent(QWheelEvent *event)
{

}

void FormRasEditor::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape && ui->leSearch->hasFocus()) {
        ui->txtEditor->setFocus();
    }

    if (e->key()==Qt::Key_W && (QApplication::keyboardModifiers() & Qt::ControlModifier))
        Data::data.requestCloseWindow = true;

    if (e->key()==Qt::Key_K && (QApplication::keyboardModifiers() & Qt::ControlModifier))
        AutoFormat();

    if (e->key()==Qt::Key_F && QApplication::keyboardModifiers() & Qt::ControlModifier) {
        ui->leSearch->setText("");
        ui->leSearch->setFocus();
        m_searchFromPos =ui->txtEditor->textCursor().position();
    }

    if (e->key()==Qt::Key_F1) {


        QTextCursor tc = ui->txtEditor->textCursor();
        tc.select(QTextCursor::WordUnderCursor);
        QString word = tc.selectedText();

        DialogHelp* dh = new DialogHelp(this, word);
        dh->show();

    }

    if (e->key() == Qt::Key_S &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        //on_btnSave_2_clicked();
        //            Save(m_currentSourceFile);
        SaveCurrent();
    }
    if (e->key() == Qt::Key_B &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        Build();
    }
    if (e->key() == Qt::Key_R &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        Build();
        Run();
    }

//    if (e->key() == Qt::Key_Tab && (QApplication::keyboardModifiers() & Qt


}


void FormRasEditor::on_leSearch_textChanged()
{
    QString i;
    SearchInSource();
}

void FormRasEditor::SearchInSource()
{
    m_currentFromPos = ui->txtEditor->document()->toPlainText().toLower().indexOf(ui->leSearch->text().toLower(), m_searchFromPos);
    QTextCursor cursor(ui->txtEditor->document()->findBlock(m_currentFromPos));
    ui->txtEditor->setTextCursor(cursor);
}

void FormRasEditor::UpdateColors()
{
    SetupHighlighter();
}

void FormRasEditor::UpdateFromIni()
{

    QFontMetrics metrics(m_font);

    m_font.setPointSize(m_iniFile->getdouble("font_size"));
    ui->txtEditor->setFont(m_font);
    ui->txtEditor->setTabStopWidth(m_iniFile->getInt("tab_width") * metrics.width(' '));


}

void FormRasEditor::AutoFormat()
{
    if (!BuildStep())
        return;

    int pos = ui->txtEditor->textCursor().position();
    QStringList source = ui->txtEditor->document()->toPlainText().split("\n");
    int ln = 0;
    int curBlock = 0;
    int nextBlock = 0;
    int singleLine = 0;
    QString outSource = "";
    bool varBlock = false;
    for (QString& s:source) {
        QString nxt = "";
        if (ln+1<m_currentSourceFile.count()-1)
            nxt = source[ln+1];
        nxt=nxt.toLower();

        int add = singleLine;
        s=s.trimmed();
        nextBlock = curBlock;
        QString k = s.toLower();
        if (k.startsWith("procedure")) {
            curBlock = 0;
            nextBlock = 0;
        }

        if (varBlock)
            if (k.startsWith("procedure") || k.startsWith("begin")) {
                varBlock = false;
                curBlock = 0;
                nextBlock = 0;
            }


        if (varBlock) {
            curBlock=1;
            nextBlock = 1;
        }
        if (k=="var")
            varBlock = true;

        if (k.contains("begin"))
            nextBlock++;
        if (k.startsWith("end")) {
            curBlock--;
            nextBlock = curBlock;
        }
        if (k.contains("if ") || k.contains("while ") || k.contains("for ") || k.contains("else")) {
            if (!k.contains("begin") && !nxt.contains("begin"))
                if (!k.endsWith(";") )
                singleLine = 1;
        }
        for (int i=0;i<curBlock+add;i++)
            s.insert(0,"\t");

        if (add!=0) {
            curBlock--;
            singleLine = 0;
        }

        ln++;
        outSource+=s+"\n";
        curBlock = nextBlock;
    }
    SetText(outSource);
    QTextCursor tc = ui->txtEditor->textCursor();
    tc.setPosition(pos);
    ui->txtEditor->setTextCursor(tc);

}

bool FormRasEditor::BuildStep()
{
    if (!m_currentSourceFile.toLower().endsWith(".ras")) {
        ui->txtOutput->setText("Turbo Rascal SE can only compile .ras files");
        m_buildSuccess = false;
        return false;
    }


    QString text = ui->txtEditor->toPlainText();
    ErrorHandler::e.m_level = ErrorHandler::e.ERROR_ONLY;
    ErrorHandler::e.m_teOut = "";
    ErrorHandler::e.exitOnError = false;
    QStringList lst = text.split("\n");
 //   text = text.replace("\n","");
//    SymbolTable::isInitialized = true;

    timer.start();
    lexer = Lexer(text, lst, m_projectIniFile->getString("project_path"));
    parser = Parser(&lexer);
    interpreter = Interpreter(&parser);
    interpreter.Parse();
//    interpreter.Interpret();

    //interpreter.Build(Interpreter::PASCAL);
    //interpreter.SaveBuild(m_outputFilename+".pmm");

    QString path = m_projectIniFile->getString("project_path") + "/";
    filename = m_currentSourceFile.split(".")[0];

    return interpreter.Build(Interpreter::MOS6502, path);
}

void FormRasEditor::FillFromIni()
{
    ui->chkPostOpt->setChecked(m_iniFile->getdouble("post_optimize")==1);
    ui->chkExomize->setChecked(m_iniFile->getdouble("perform_crunch")==1);
}

void FormRasEditor::FillToIni()
{
    if (ui->chkPostOpt->isChecked())
        m_iniFile->setFloat("post_optimize",1);
    else
        m_iniFile->setFloat("post_optimize",0);

    if (ui->chkExomize->isChecked())
        m_iniFile->setFloat("perform_crunch",1);
    else
        m_iniFile->setFloat("perform_crunch",0);


    m_iniFile->Save();
}

void FormRasEditor::Reload()
{
    if (!m_currentSourceFile.contains(".asm"))
        return;
    int pos = ui->txtEditor->textCursor().position();

    Load(m_currentSourceFile);
    QTextCursor tc = ui->txtEditor->textCursor();
    tc.setPosition(pos);
    ui->txtEditor->setTextCursor(tc);

}

void FormRasEditor::Save(QString filename)
{
    if (QFile::exists(filename))
        QFile::remove(filename);
    QString txt = ui->txtEditor->document()->toPlainText();
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << txt;
    }
    file.close();
    m_iniFile->Save();

}

void FormRasEditor::Load(QString filename)
{
    QFile file(filename);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        SetText(file.readAll());
    }
    file.close();

}



void FormRasEditor::on_leSearch_returnPressed()
{
    m_searchFromPos=m_currentFromPos+1;
    SearchInSource();

}

void FormRasEditor::on_leSearch_textChanged(const QString &arg1)
{
    SearchInSource();
}

void FormRasEditor::on_btnReplace_clicked()
{
    QString orgstr = ui->leSearch->text();
    QString replacestr = ui->leReplace->text();

    QString source = ui->txtEditor->toPlainText();
    source = source.replace(orgstr, replacestr);

    SetText(source);

}

void FormRasEditor::on_chkExomize_stateChanged(int arg1)
{
    FillToIni();
}

void FormRasEditor::on_chkPostOpt_stateChanged(int arg1)
{
    FillToIni();
}
