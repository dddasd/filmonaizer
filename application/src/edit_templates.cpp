#include "edit_templates.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    tagsFormat.setFontWeight(QFont::Bold);
    tagsFormat.setForeground(Qt::darkBlue);

    globaltagsFormat.setFontWeight(QFont::Bold);
    globaltagsFormat.setForeground(Qt::darkMagenta);

    HighlightingRule hh;

    hh.format = tagsFormat;
    hh.pattern = QRegExp("\\{:([^\\}\\{$]*)\\}");
    highlightingRules.append(hh);

    hh.format = globaltagsFormat;
    hh.pattern = QRegExp("\\{;([^\\}\\{$]*)\\}");
    highlightingRules.append(hh);
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);
}

Edit_Templates::Edit_Templates(QString dir, QString file_templates, QList<QString> tags_global, QList<QString> tags_plug_search, QList<QString> tags_plug_mediainfo, QWidget *parent)
    : QMainWindow(parent)
{
    if (!file_templates.isEmpty())
        tek_file = dir + QDir::separator() + file_templates;
    else
        tek_file = "";
    dir_file = dir;

    this->setObjectName(QString::fromUtf8("form_set_templates"));
    this->setWindowTitle(QApplication::translate("form_set_templates", "Edit templates", 0));
    QFont font;
    font.setPointSize(10);
    this->setFont(font);

    this->setWindowIcon(QIcon(":icons/scripts--pencil.png"));

    text_templates = new QPlainTextEdit(this);
    text_templates->setObjectName(QString::fromUtf8("text_templates"));

    this->setCentralWidget(text_templates);

    tool_new = new QToolButton(this);
    tool_new->setToolTip(QApplication::translate("form_set_templates", "New", 0));
    tool_new->setText("");
    tool_new->setMinimumSize(23,23);
    tool_new->setMaximumSize(23,23);
    tool_new->setIcon(QIcon(":icons/document.png"));
    connect(tool_new,SIGNAL(clicked()),this,SLOT(new_file()));

    tool_open = new QToolButton(this);
    tool_open->setToolTip(QApplication::translate("form_set_templates", "Open", 0));
    tool_open->setText("");
    tool_open->setMinimumSize(23,23);
    tool_open->setMaximumSize(23,23);
    tool_open->setIcon(QIcon(":icons/folder-horizontal-open.png"));

    tool_save = new QToolButton(this);
    tool_save->setToolTip(QApplication::translate("form_set_templates", "Save", 0));
    tool_save->setText("");
    tool_save->setMinimumSize(23,23);
    tool_save->setMaximumSize(23,23);
    tool_save->setIcon(QIcon(":icons/disk-black.png"));
    connect(tool_save,SIGNAL(clicked()),this,SLOT(save_file()));

    tool_delete_file = new QToolButton(this);
    tool_delete_file->setToolTip(QApplication::translate("form_set_templates", "Delete file", 0));
    tool_delete_file->setText("");
    tool_delete_file->setMinimumSize(23,23);
    tool_delete_file->setMaximumSize(23,23);
    tool_delete_file->setIcon(QIcon(":icons/cross-circle.png"));
    connect(tool_delete_file,SIGNAL(clicked()),this,SLOT(delete_file()));

    tool_cut = new QToolButton(this);
    tool_cut->setToolTip(QApplication::translate("form_set_templates", "Cut", 0));
    tool_cut->setText("");
    tool_cut->setMinimumSize(23,23);
    tool_cut->setMaximumSize(23,23);
    tool_cut->setIcon(QIcon(":icons/scissors-blue.png"));
    connect(tool_cut,SIGNAL(clicked()),text_templates,SLOT(cut()));

    tool_copy = new QToolButton(this);
    tool_copy->setToolTip(QApplication::translate("form_set_templates", "Copy", 0));
    tool_copy->setText("");
    tool_copy->setMinimumSize(23,23);
    tool_copy->setMaximumSize(23,23);
    tool_copy->setIcon(QIcon(":icons/document-copy.png"));
    connect(tool_copy,SIGNAL(clicked()),text_templates,SLOT(copy()));

    tool_paste = new QToolButton(this);
    tool_paste->setToolTip(QApplication::translate("form_set_templates", "Paste", 0));
    tool_paste->setText("");
    tool_paste->setMinimumSize(23,23);
    tool_paste->setMaximumSize(23,23);
    tool_paste->setIcon(QIcon(":icons/clipboard-sign-out.png"));
    connect(tool_paste,SIGNAL(clicked()),text_templates,SLOT(paste()));

    tool_undo = new QToolButton(this);
    tool_undo->setToolTip(QApplication::translate("form_set_templates", "Undo", 0));
    tool_undo->setText("");
    tool_undo->setMinimumSize(23,23);
    tool_undo->setMaximumSize(23,23);
    tool_undo->setIcon(QIcon(":icons/arrow-curve-180-left.png"));
    connect(tool_undo,SIGNAL(clicked()),text_templates,SLOT(undo()));

    tool_redo = new QToolButton(this);
    tool_redo->setToolTip(QApplication::translate("form_set_templates", "Redo", 0));
    tool_redo->setText("");
    tool_redo->setMinimumSize(23,23);
    tool_redo->setMaximumSize(23,23);
    tool_redo->setIcon(QIcon(":icons/arrow-curve.png"));
    connect(tool_redo,SIGNAL(clicked()),text_templates,SLOT(redo()));

    tool_paste_tags = new QToolButton(this);
    tool_paste_tags->setToolTip(QApplication::translate("form_set_templates", "Paste variables", 0));
    tool_paste_tags->setText("");
    tool_paste_tags->setMinimumSize(23,23);
    tool_paste_tags->setMaximumSize(23,23);
    tool_paste_tags->setIcon(QIcon(":icons/document-code.png"));

    tool_paste_symbol = new QToolButton(this);
    tool_paste_symbol->setToolTip(QApplication::translate("form_set_templates", "Paste symbols", 0));
    tool_paste_symbol->setText("");
    tool_paste_symbol->setMinimumSize(23,23);
    tool_paste_symbol->setMaximumSize(23,23);
    tool_paste_symbol->setIcon(QIcon(":icons/document-number.png"));

    tool_help = new QToolButton(this);
    tool_help->setToolTip(QApplication::translate("form_set_templates", "Open help", 0));
    tool_help->setText("");
    tool_help->setMinimumSize(23,23);
    tool_help->setMaximumSize(23,23);
    tool_help->setIcon(QIcon(":icons/question.png"));
    connect(tool_help,SIGNAL(clicked()),this,SLOT(open_help()));

    bar_file = addToolBar(QApplication::translate("form_set_templates", "Toolbar file", 0));
    bar_copypaste = addToolBar(QApplication::translate("form_set_templates", "Toolbar buffer", 0));
    bar_undoredo = addToolBar(QApplication::translate("form_set_templates", "Toolbar history", 0));
    bar_paste_ = addToolBar(QApplication::translate("form_set_templates", "Toolbar variables", 0));
    bar_file->addWidget(tool_new);
    bar_file->addWidget(tool_open);
    bar_file->addWidget(tool_save);
    bar_file->addWidget(tool_delete_file);
    //bar_main->addSeparator();
    bar_copypaste->addWidget(tool_cut);
    bar_copypaste->addWidget(tool_copy);
    bar_copypaste->addWidget(tool_paste);
    //bar_main->addSeparator();
    bar_undoredo->addWidget(tool_undo);
    bar_undoredo->addWidget(tool_redo);
    //bar_main->addSeparator();
    bar_paste_->addWidget(tool_paste_tags);
    bar_paste_->addWidget(tool_paste_symbol);
    bar_paste_->addWidget(tool_help);

    menu_open = new QMenu;
    create_menu_open();

    menu_tags = new QMenu;
	menu_local_variables = new QMenu;
	menu_local_variables->setTitle(tr("Local Variables"));
	menu_global_variables = new QMenu;
	menu_global_variables->setTitle(tr("Global Variables"));
	menu_mediainfo_variables = new QMenu;
	menu_mediainfo_variables->setTitle(tr("MediaInfo Variables"));
	menu_tags->addMenu(menu_global_variables);
	menu_tags->addMenu(menu_local_variables);
    menu_tags->addMenu(menu_mediainfo_variables);

    for (int i=0;i<tags_global.length();i++) menu_global_variables->addAction(tags_global[i]);

    for (int i=0;i<tags_plug_search.length();i++) menu_local_variables->addAction(tags_plug_search[i]);

    for (int i=0;i<tags_plug_mediainfo.length();i++) menu_mediainfo_variables->addAction(tags_plug_mediainfo[i]);

    tool_paste_tags->setMenu(menu_tags);
    tool_paste_tags->setPopupMode(QToolButton::InstantPopup);
    connect(tool_paste_tags,SIGNAL(triggered(QAction*)),this,SLOT(_triggered_tags(QAction*)));

    menu_symbol = new QMenu;
    menu_symbol->addAction("{");
    menu_symbol->addAction("}");
    tool_paste_symbol->setMenu(menu_symbol);
    tool_paste_symbol->setPopupMode(QToolButton::InstantPopup);
    connect(tool_paste_symbol,SIGNAL(triggered(QAction*)),this,SLOT(_triggered_symbol(QAction*)));

    highlighter = new Highlighter(text_templates->document());

    if (tek_file.isEmpty()) {
        setWindowTitle(QString(tr("%1 - Edit templates")).arg(tr("New")));
    }
    else {
        QFile file(tek_file);
        if (file.open(QIODevice::ReadOnly)) {
            text_templates->setPlainText(QString::fromUtf8(file.readAll().data()));
            file.close();
            setWindowTitle(QString(tr("%1 - Edit templates")).arg(QFileInfo(file_templates).fileName()));
        }
        else {
            setWindowTitle(QString(tr("%1 - Edit templates")).arg(tr("New")));
        }
    }

    connect(text_templates,SIGNAL(modificationChanged(bool)),this,SLOT(modif(bool)));
    connect(text_templates,SIGNAL(redoAvailable(bool)),tool_redo,SLOT(setEnabled(bool)));
    connect(text_templates,SIGNAL(undoAvailable(bool)),tool_undo,SLOT(setEnabled(bool)));

    tool_save->setEnabled(false);
    tool_undo->setEnabled(false);
    tool_redo->setEnabled(false);

    resize(500,300);
}

void Edit_Templates::create_menu_open()
{
    disconnect(tool_open,SIGNAL(triggered(QAction*)),this,SLOT(_triggered_open(QAction*)));
    menu_open->clear();
    QDir dir;
    dir.setPath(dir_file);
    QStringList list_f;
    list_f << "*.txt";
    QList<QString> list = dir.entryList(list_f,QDir::Files);
    for (int i=0;i<list.length();i++)
        menu_open->addAction(list[i]);
    menu_open->addSeparator();
    menu_open->addAction(QIcon(":icons/folder--plus.png"),tr("Add existing file template"));
    tool_open->setMenu(menu_open);
    tool_open->setPopupMode(QToolButton::InstantPopup);
    connect(tool_open,SIGNAL(triggered(QAction*)),this,SLOT(_triggered_open(QAction*)));
}

void Edit_Templates::modif(bool b)
{
    tool_save->setEnabled(b);
    if (b)
    {
        if (tek_file.isEmpty())
            setWindowTitle(QString(tr("%1* - Edit templates")).arg(tr("New")));
        else
            setWindowTitle(QString(tr("%1* - Edit templates")).arg(QFileInfo(tek_file).fileName()));
    }
    else {
        if (tek_file.isEmpty())
            setWindowTitle(QString(tr("%1 - Edit templates")).arg(tr("New")));
        else
            setWindowTitle(QString(tr("%1 - Edit templates")).arg(QFileInfo(tek_file).fileName()));
    }
}

void Edit_Templates::_triggered_open(QAction* ret)
{
    if ((ret==menu_open->actions()[menu_open->actions().length()-1]) || (ret->isSeparator())) {
        if (!ret->isSeparator()) {
            QString filename = QString::fromLocal8Bit(QFileDialog::getOpenFileName(this, tr("Select a templates file"),
                                                         qApp->applicationDirPath(),
                                                         tr("All Files (*.*)")).toLocal8Bit());
            if (!filename.isEmpty()) {
                QString newname = dir_file+QDir::separator()+QFileInfo(filename).fileName();
                if ((QFileInfo(newname).suffix().isEmpty()) || (QFileInfo(newname).suffix()!="txt"))
                    newname += ".txt";
                if (QFile::exists(newname)) {
                    QMessageBox msgBox1(this);
                    msgBox1.setText(tr("File is exist!"));
                    msgBox1.setInformativeText(tr("Rewrite?"));
                    msgBox1.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
                    msgBox1.setButtonText(QMessageBox::Yes,tr("Yes"));
                    msgBox1.setButtonText(QMessageBox::No,tr("No"));
                    msgBox1.setButtonText(QMessageBox::Cancel,tr("Cancel"));
                    msgBox1.setDefaultButton(QMessageBox::Yes);
                    int ret2 = msgBox1.exec();
                    switch (ret2) {
                        case QMessageBox::Yes: {
                            if (QFile::remove(newname)) {
                                if (!QFile::copy(filename,newname)) {
                                    QMessageBox msgBox1(this);
                                    msgBox1.setText(tr("Unable to create the file!"));
                                    msgBox1.setIcon(QMessageBox::Warning);
                                    msgBox1.setStandardButtons(QMessageBox::Ok);
                                    msgBox1.exec();
                                }
                            }
                            else {
                                QMessageBox msgBox1(this);
                                msgBox1.setText(tr("Unable to copy the file!"));
                                msgBox1.setIcon(QMessageBox::Warning);
                                msgBox1.setStandardButtons(QMessageBox::Ok);
                                msgBox1.exec();
                            }
                            break;
                        }
                        case QMessageBox::No: {
                            QString fileName = "";
                            for (int i=0;;i++) {
                                if (!QFile::exists(dir_file+QDir::separator()+QString("NewTemplate%1.txt").arg(i))) {
                                    fileName = dir_file+QDir::separator()+QString("NewTemplate%1.txt").arg(i);
                                    break;
                                }
                            }
                            bool ok;
                            QString text = QString::fromLocal8Bit(QInputDialog::getText(this, tr("Enter a name for the new template."),
                                                                 "", QLineEdit::Normal,QFileInfo(fileName).fileName(), &ok).toLocal8Bit());
                            if (ok && !text.isEmpty()) {
                                if (QFileInfo(dir_file+QDir::separator()+text).suffix().isEmpty())
                                    text += ".txt";
                                QFile file(dir_file+QDir::separator()+text);
                                if (file.exists(file.fileName())) {
                                    QMessageBox msgBox1(this);
                                    msgBox1.setText(tr("File is exist!"));
                                    msgBox1.setInformativeText(tr("Rewrite?"));
                                    msgBox1.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
                                    msgBox1.setButtonText(QMessageBox::Yes,tr("Yes"));
                                    msgBox1.setButtonText(QMessageBox::No,tr("No"));
                                    msgBox1.setButtonText(QMessageBox::Cancel,tr("Cancel"));
                                    msgBox1.setDefaultButton(QMessageBox::Yes);
                                    int ret2 = msgBox1.exec();
                                    switch (ret2) {
                                        case QMessageBox::Yes: {
                                            if (QFile::remove(file.fileName())) {
                                                if (!QFile::copy(filename,file.fileName())) {
                                                    QMessageBox msgBox1(this);
                                                    msgBox1.setText(tr("Unable to create the file!"));
                                                    msgBox1.setIcon(QMessageBox::Warning);
                                                    msgBox1.setStandardButtons(QMessageBox::Ok);
                                                    msgBox1.exec();
                                                }
                                            }
                                            else {
                                                QMessageBox msgBox1(this);
                                                msgBox1.setText(tr("Unable to copy the file!"));
                                                msgBox1.setIcon(QMessageBox::Warning);
                                                msgBox1.setStandardButtons(QMessageBox::Ok);
                                                msgBox1.exec();
                                            }
                                            break;
                                        }
                                        case QMessageBox::No: {                                            
                                            break;
                                        }
                                        case QMessageBox::Cancel: {
                                            break;
                                        }
                                        default: {
                                            break;
                                        }
                                    }
                                }
                                else {
                                    if (!QFile::copy(filename,file.fileName())) {
                                        QMessageBox msgBox1(this);
                                        msgBox1.setText(tr("Unable to create the file!"));
                                        msgBox1.setIcon(QMessageBox::Warning);
                                        msgBox1.setStandardButtons(QMessageBox::Ok);
                                        msgBox1.exec();
                                    }
                                }
                            }
                            break;
                        }
                        case QMessageBox::Cancel: {
                            break;
                        }
                        default: {
                            break;
                        }
                    }
                }
                else {
                    if (!QFile::copy(filename,newname)) {
                        QMessageBox msgBox1(this);
                        msgBox1.setText(tr("Unable to create the file!"));
                        msgBox1.setIcon(QMessageBox::Warning);
                        msgBox1.setStandardButtons(QMessageBox::Ok);
                        msgBox1.exec();
                    }
                }
            }
        }
    }
    else {
        if (tool_save->isEnabled()) {
            QMessageBox msgBox(this);
            msgBox.setText(tr("The file is modified."));
            msgBox.setInformativeText(tr("Save?"));
            msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel | QMessageBox::SaveAll);
            msgBox.setButtonText(QMessageBox::Save,tr("Save"));
            msgBox.setButtonText(QMessageBox::No,tr("No"));
            msgBox.setButtonText(QMessageBox::Cancel,tr("Cancel"));
            msgBox.setButtonText(QMessageBox::SaveAll,tr("Save as..."));
            msgBox.setDefaultButton(QMessageBox::Save);
            int ret1 = msgBox.exec();
            switch (ret1) {
                case QMessageBox::Save: {
                    QFile file(tek_file);
                    if (file.open(QIODevice::WriteOnly)) {
                        if (file.write(text_templates->toPlainText().toUtf8())!=text_templates->toPlainText().toUtf8().length()) {
                            file.close();
                            QMessageBox msgBox1(this);
                            msgBox1.setText(tr("Unable to save the file!"));
                            msgBox1.setIcon(QMessageBox::Warning);
                            msgBox1.setStandardButtons(QMessageBox::Ok);
                            msgBox1.exec();
                        }
                        else {
                            file.close();
                            file.setFileName(dir_file+QDir::separator()+ret->text());
                            if (file.exists(file.fileName())) {
                                if (file.open(QIODevice::ReadOnly)) {
                                    text_templates->setPlainText(QString::fromUtf8(file.readAll().data()));
                                    file.close();

                                    tek_file = dir_file +QDir::separator()+ret->text();
                                    text_templates->document()->setModified(false);
                                    text_templates->document()->setUndoRedoEnabled(false);
                                    text_templates->document()->setUndoRedoEnabled(true);
                                    tool_save->setEnabled(false);
                                    tool_undo->setEnabled(false);
                                    tool_redo->setEnabled(false);
                                    modif(false);
                                }
                                else {
                                    QMessageBox msgBox1(this);
                                    msgBox1.setText(tr("Unable to open the file!"));
                                    msgBox1.setIcon(QMessageBox::Warning);
                                    msgBox1.setStandardButtons(QMessageBox::Ok);
                                    msgBox1.exec();
                                }
                            }
                            else {
                                QMessageBox msgBox1(this);
                                msgBox1.setText(tr("Such a file does not exist!"));
                                msgBox1.setIcon(QMessageBox::Warning);
                                msgBox1.setStandardButtons(QMessageBox::Ok);
                                msgBox1.exec();
                            }
                        }
                    }
                    else {
                        QMessageBox msgBox1(this);
                        msgBox1.setText(tr("Unable to open the file!"));
                        msgBox1.setIcon(QMessageBox::Warning);
                        msgBox1.setStandardButtons(QMessageBox::Ok);
                        msgBox1.exec();
                    }
                    break;
                }
                case QMessageBox::No: {
                    QFile file(dir_file+QDir::separator()+ret->text());
                    if (file.exists(file.fileName())) {
                        if (file.open(QIODevice::ReadOnly)) {
                            text_templates->setPlainText(QString::fromUtf8(file.readAll().data()));
                            file.close();

                            tek_file = dir_file +QDir::separator()+ret->text();
                            text_templates->document()->setModified(false);
                            text_templates->document()->setUndoRedoEnabled(false);
                            text_templates->document()->setUndoRedoEnabled(true);
                            tool_save->setEnabled(false);
                            tool_undo->setEnabled(false);
                            tool_redo->setEnabled(false);
                            modif(false);
                        }
                        else {
                            QMessageBox msgBox1(this);
                            msgBox1.setText(tr("Unable to open the file!"));
                            msgBox1.setIcon(QMessageBox::Warning);
                            msgBox1.setStandardButtons(QMessageBox::Ok);
                            msgBox1.exec();
                        }
                    }
                    else {
                        QMessageBox msgBox1(this);
                        msgBox1.setText(tr("Such a file does not exist!"));
                        msgBox1.setIcon(QMessageBox::Warning);
                        msgBox1.setStandardButtons(QMessageBox::Ok);
                        msgBox1.exec();
                    }
                    break;
                }
                case QMessageBox::Cancel: {
                    break;
                }
                case QMessageBox::SaveAll: {
                    QString fileName = "";
                    for (int i=0;;i++) {
                        if (!QFile::exists(dir_file+QDir::separator()+QString("NewTemplate%1.txt").arg(i))) {
                            fileName = dir_file+QDir::separator()+QString("NewTemplate%1.txt").arg(i);
                            break;
                        }
                    }
                    bool ok;
                    QString text = QString::fromLocal8Bit(QInputDialog::getText(this, tr("Enter a name for the new template."),
                                                          "", QLineEdit::Normal,QFileInfo(fileName).fileName(), &ok).toLocal8Bit());
                    if (ok && !text.isEmpty()) {
                        if (QFileInfo(dir_file+QDir::separator()+text).suffix().isEmpty())
                            text += ".txt";
                        QFile file(dir_file+QDir::separator()+text);
                        if (file.exists(file.fileName())) {
                            QMessageBox msgBox1(this);
                            msgBox1.setText(tr("File is exist!"));
                            msgBox1.setInformativeText(tr("Rewrite?"));
                            msgBox1.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
                            msgBox1.setButtonText(QMessageBox::Yes,tr("Yes"));
                            msgBox1.setButtonText(QMessageBox::No,tr("No"));
                            msgBox1.setButtonText(QMessageBox::Cancel,tr("Cancel"));
                            msgBox1.setDefaultButton(QMessageBox::Yes);
                            int ret2 = msgBox1.exec();
                            switch (ret2) {
                                case QMessageBox::Yes: {
                                    if (file.open(QIODevice::WriteOnly)) {
                                        if (file.write(text_templates->toPlainText().toUtf8())!=text_templates->toPlainText().toUtf8().length()) {
                                            file.close();
                                            QMessageBox msgBox1(this);
                                            msgBox1.setText(tr("Unable to save the file!"));
                                            msgBox1.setIcon(QMessageBox::Warning);
                                            msgBox1.setStandardButtons(QMessageBox::Ok);
                                            msgBox1.exec();
                                        }
                                        else {
                                            file.close();
                                            file.setFileName(dir_file+QDir::separator()+ret->text());
                                            if (file.exists(file.fileName())) {
                                                if (file.open(QIODevice::ReadOnly)) {
                                                    text_templates->setPlainText(QString::fromUtf8(file.readAll().data()));
                                                    file.close();

                                                    tek_file = dir_file +QDir::separator()+ret->text();
                                                    text_templates->document()->setModified(false);
                                                    text_templates->document()->setUndoRedoEnabled(false);
                                                    text_templates->document()->setUndoRedoEnabled(true);
                                                    tool_save->setEnabled(false);
                                                    tool_undo->setEnabled(false);
                                                    tool_redo->setEnabled(false);
                                                    modif(false);
                                                }
                                                else {
                                                    QMessageBox msgBox1(this);
                                                    msgBox1.setText(tr("Unable to open the file!"));
                                                    msgBox1.setIcon(QMessageBox::Warning);
                                                    msgBox1.setStandardButtons(QMessageBox::Ok);
                                                    msgBox1.exec();
                                                }
                                            }
                                            else {
                                                QMessageBox msgBox1(this);
                                                msgBox1.setText(tr("Such a file does not exist!"));
                                                msgBox1.setIcon(QMessageBox::Warning);
                                                msgBox1.setStandardButtons(QMessageBox::Ok);
                                                msgBox1.exec();
                                            }
                                        }
                                    }
                                    else {
                                        QMessageBox msgBox1(this);
                                        msgBox1.setText(tr("Unable to open the file!"));
                                        msgBox1.setIcon(QMessageBox::Warning);
                                        msgBox1.setStandardButtons(QMessageBox::Ok);
                                        msgBox1.exec();
                                    }
                                    break;
                                }
                                case QMessageBox::No: {
                                    QFile file(dir_file+QDir::separator()+ret->text());
                                    if (file.exists(file.fileName())) {
                                        if (file.open(QIODevice::ReadOnly)) {
                                            text_templates->setPlainText(QString::fromUtf8(file.readAll().data()));
                                            file.close();

                                            tek_file = dir_file +QDir::separator()+ret->text();
                                            text_templates->document()->setModified(false);
                                            text_templates->document()->setUndoRedoEnabled(false);
                                            text_templates->document()->setUndoRedoEnabled(true);
                                            tool_save->setEnabled(false);
                                            tool_undo->setEnabled(false);
                                            tool_redo->setEnabled(false);
                                            modif(false);
                                        }
                                        else {
                                            QMessageBox msgBox1(this);
                                            msgBox1.setText(tr("Unable to open the file!"));
                                            msgBox1.setIcon(QMessageBox::Warning);
                                            msgBox1.setStandardButtons(QMessageBox::Ok);
                                            msgBox1.exec();
                                        }
                                    }
                                    else {
                                        QMessageBox msgBox1(this);
                                        msgBox1.setText(tr("Such a file does not exist!"));
                                        msgBox1.setIcon(QMessageBox::Warning);
                                        msgBox1.setStandardButtons(QMessageBox::Ok);
                                        msgBox1.exec();
                                    }
                                    break;
                                }
                                case QMessageBox::Cancel: {
                                    break;
                                }
                                default: {
                                    break;
                                }
                            }
                        }
                        else {
                            if (file.open(QIODevice::WriteOnly)) {
                                if (file.write(text_templates->toPlainText().toUtf8())!=text_templates->toPlainText().toUtf8().length()) {
                                    QMessageBox msgBox1(this);
                                    msgBox1.setText(tr("Unable to save the file!"));
                                    msgBox1.setIcon(QMessageBox::Warning);
                                    msgBox1.setStandardButtons(QMessageBox::Ok);
                                    msgBox1.exec();
                                    file.close();
                                }
                                else {
                                    file.close();
                                    QFile file(dir_file+QDir::separator()+ret->text());
                                    if (file.exists(file.fileName())) {
                                        if (file.open(QIODevice::ReadOnly)) {
                                            text_templates->setPlainText(QString::fromUtf8(file.readAll().data()));
                                            file.close();

                                            tek_file = dir_file +QDir::separator()+ret->text();
                                            text_templates->document()->setModified(false);
                                            text_templates->document()->setUndoRedoEnabled(false);
                                            text_templates->document()->setUndoRedoEnabled(true);
                                            tool_save->setEnabled(false);
                                            tool_undo->setEnabled(false);
                                            tool_redo->setEnabled(false);
                                            modif(false);
                                        }
                                        else {
                                            QMessageBox msgBox1(this);
                                            msgBox1.setText(tr("Unable to open the file!"));
                                            msgBox1.setIcon(QMessageBox::Warning);
                                            msgBox1.setStandardButtons(QMessageBox::Ok);
                                            msgBox1.exec();
                                        }
                                    }
                                    else {
                                        QMessageBox msgBox1(this);
                                        msgBox1.setText(tr("Such a file does not exist!"));
                                        msgBox1.setIcon(QMessageBox::Warning);
                                        msgBox1.setStandardButtons(QMessageBox::Ok);
                                        msgBox1.exec();
                                    }
                                }
                            }

                        }
                    }

                    break;
                }
                default: {
                    break;
                }
            }
        }
        else {
            QFile file(dir_file+QDir::separator()+ret->text());
            if (file.exists(file.fileName())) {
                if (file.open(QIODevice::ReadOnly)) {
                    text_templates->setPlainText(QString::fromUtf8(file.readAll().data()));
                    file.close();

                    tek_file = dir_file +QDir::separator()+ret->text();
                    text_templates->document()->setModified(false);
                    text_templates->document()->setUndoRedoEnabled(false);
                    text_templates->document()->setUndoRedoEnabled(true);
                    tool_save->setEnabled(false);
                    tool_undo->setEnabled(false);
                    tool_redo->setEnabled(false);
                    modif(false);
                }
                else {
                    QMessageBox msgBox(this);
                    msgBox.setText(tr("Unable to open the file!"));
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.exec();
                }
            }
            else {
                QMessageBox msgBox(this);
                msgBox.setText(tr("Such a file does not exist!"));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
    }
    create_menu_open();
}

void Edit_Templates::_triggered_tags(QAction* ret)
{
    QTextCursor cur = text_templates->textCursor();
	if(ret->text()!="{;custom}") {
		int i = cur.position();
		cur.insertText(ret->text());
		if(ret->text()=="{custom}")
			cur.setPosition(i+7);
	}
	else {
		int i = cur.position();
		cur.insertText("{;custom=(name=\"\")(item=\"\")}");
		cur.setPosition(i+8);
	}
	text_templates->setTextCursor(cur);
}

void Edit_Templates::_triggered_symbol(QAction* ret)
{
    QTextCursor cur = text_templates->textCursor();
    if (ret->text()=="{")
        cur.insertText("&#123;");
    else
        cur.insertText("&#125;");
    text_templates->setTextCursor(cur);
}

void Edit_Templates::new_file()
{
    if (tool_save->isEnabled()) {
        QMessageBox msgBox(this);
        msgBox.setText(tr("The file is modified."));
        msgBox.setInformativeText(tr("Save?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel | QMessageBox::SaveAll);
        msgBox.setButtonText(QMessageBox::Save,tr("Save"));
        msgBox.setButtonText(QMessageBox::No,tr("No"));
        msgBox.setButtonText(QMessageBox::Cancel,tr("Cancel"));
        msgBox.setButtonText(QMessageBox::SaveAll,tr("Save as..."));
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret1 = msgBox.exec();
        switch (ret1) {
            case QMessageBox::Save: {
                if (tek_file.isEmpty()) {
                    QString fileName = "";
                    for (int i=0;;i++) {
                        if (!QFile::exists(dir_file+QDir::separator()+QString("NewTemplate%1.txt").arg(i))) {
                            fileName = dir_file+QDir::separator()+QString("NewTemplate%1.txt").arg(i);
                            break;
                        }
                    }
                    bool ok;
                    QString text = QString::fromLocal8Bit(QInputDialog::getText(this, tr("Enter a name for the new template."),
                                                                                "", QLineEdit::Normal,QFileInfo(fileName).fileName(), &ok).toLocal8Bit());
                    if (ok && !text.isEmpty()) {
                        if (QFileInfo(dir_file+QDir::separator()+text).suffix().isEmpty())
                            text += ".txt";
                        QFile file(dir_file+QDir::separator()+text);
                        if (file.exists(file.fileName())) {
                            QMessageBox msgBox1(this);
                            msgBox1.setText(tr("File is exist!"));
                            msgBox1.setInformativeText(tr("Rewrite?"));
                            msgBox1.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
                            msgBox1.setButtonText(QMessageBox::Yes,tr("Yes"));
                            msgBox1.setButtonText(QMessageBox::No,tr("No"));
                            msgBox1.setButtonText(QMessageBox::Cancel,tr("Cancel"));
                            msgBox1.setDefaultButton(QMessageBox::Yes);
                            int ret2 = msgBox1.exec();
                            switch (ret2) {
                                case QMessageBox::Yes: {
                                    if (file.open(QIODevice::WriteOnly)) {
                                        if (file.write(text_templates->toPlainText().toUtf8())!=text_templates->toPlainText().toUtf8().length()) {
                                            file.close();
                                            QMessageBox msgBox1(this);
                                            msgBox1.setText(tr("Unable to save the file!"));
                                            msgBox1.setIcon(QMessageBox::Warning);
                                            msgBox1.setStandardButtons(QMessageBox::Ok);
                                            msgBox1.exec();
                                        }
                                        else {
                                            file.close();
                                            text_templates->clear();
                                            tek_file = "";
                                            text_templates->document()->setModified(false);
                                            text_templates->document()->setUndoRedoEnabled(false);
                                            text_templates->document()->setUndoRedoEnabled(true);
                                            tool_save->setEnabled(false);
                                            tool_undo->setEnabled(false);
                                            tool_redo->setEnabled(false);
                                            modif(false);
                                        }
                                    }
                                    else {
                                        QMessageBox msgBox1(this);
                                        msgBox1.setText(tr("Unable to open the file!"));
                                        msgBox1.setIcon(QMessageBox::Warning);
                                        msgBox1.setStandardButtons(QMessageBox::Ok);
                                        msgBox1.exec();
                                    }
                                    break;
                                }
                                case QMessageBox::No: {
                                    text_templates->clear();
                                    tek_file = "";
                                    text_templates->document()->setModified(false);
                                    text_templates->document()->setUndoRedoEnabled(false);
                                    text_templates->document()->setUndoRedoEnabled(true);
                                    tool_save->setEnabled(false);
                                    tool_undo->setEnabled(false);
                                    tool_redo->setEnabled(false);
                                    modif(false);
                                    break;
                                }
                                case QMessageBox::Cancel: {
                                    break;
                                }
                                default: {
                                    break;
                                }
                            }
                        }
                        else {
                            if (file.open(QIODevice::WriteOnly)) {
                                if (file.write(text_templates->toPlainText().toUtf8())!=text_templates->toPlainText().toUtf8().length()) {
                                    QMessageBox msgBox1(this);
                                    msgBox1.setText(tr("Unable to save the file!"));
                                    msgBox1.setIcon(QMessageBox::Warning);
                                    msgBox1.setStandardButtons(QMessageBox::Ok);
                                    msgBox1.exec();
                                    file.close();
                                }
                                else {
                                    file.close();
                                    text_templates->clear();
                                    tek_file = "";
                                    text_templates->document()->setModified(false);
                                    text_templates->document()->setUndoRedoEnabled(false);
                                    text_templates->document()->setUndoRedoEnabled(true);
                                    tool_save->setEnabled(false);
                                    tool_undo->setEnabled(false);
                                    tool_redo->setEnabled(false);
                                    modif(false);
                                }
                            }

                        }
                    }
                }
                else {
                    QFile file(tek_file);
                    if (file.open(QIODevice::WriteOnly)) {
                        if (file.write(text_templates->toPlainText().toUtf8())!=text_templates->toPlainText().toUtf8().length()) {
                            file.close();
                            QMessageBox msgBox1(this);
                            msgBox1.setText(tr("Unable to save the file!"));
                            msgBox1.setIcon(QMessageBox::Warning);
                            msgBox1.setStandardButtons(QMessageBox::Ok);
                            msgBox1.exec();
                        }
                        else {
                            file.close();
                            text_templates->clear();
                            tek_file = "";
                            text_templates->document()->setModified(false);
                            text_templates->document()->setUndoRedoEnabled(false);
                            text_templates->document()->setUndoRedoEnabled(true);
                            tool_save->setEnabled(false);
                            tool_undo->setEnabled(false);
                            tool_redo->setEnabled(false);
                            modif(false);
                        }
                    }
                    else {
                        QMessageBox msgBox1(this);
                        msgBox1.setText(tr("Unable to open the file!"));
                        msgBox1.setIcon(QMessageBox::Warning);
                        msgBox1.setStandardButtons(QMessageBox::Ok);
                        msgBox1.exec();
                    }
                }
                break;
            }
            case QMessageBox::No: {
                text_templates->clear();
                tek_file = "";
                text_templates->document()->setModified(false);
                text_templates->document()->setUndoRedoEnabled(false);
                text_templates->document()->setUndoRedoEnabled(true);
                tool_save->setEnabled(false);
                tool_undo->setEnabled(false);
                tool_redo->setEnabled(false);
                modif(false);
                break;
            }
            case QMessageBox::Cancel: {
                break;
            }
            case QMessageBox::SaveAll: {
                QString fileName = "";
                for (int i=0;;i++) {
                    if (!QFile::exists(dir_file+QDir::separator()+QString("NewTemplate%1.txt").arg(i))) {
                        fileName = dir_file+QDir::separator()+QString("NewTemplate%1.txt").arg(i);
                        break;
                    }
                }
                bool ok;
                QString text = QString::fromLocal8Bit(QInputDialog::getText(this, tr("Enter a name for the new template."),
                                                                            "", QLineEdit::Normal,QFileInfo(fileName).fileName(), &ok).toLocal8Bit());
                if (ok && !text.isEmpty()) {
                    if (QFileInfo(dir_file+QDir::separator()+text).suffix().isEmpty())
                        text += ".txt";
                    QFile file(dir_file+QDir::separator()+text);
                    if (file.exists(file.fileName())) {
                        QMessageBox msgBox1(this);
                        msgBox1.setText(tr("File is exist!"));
                        msgBox1.setInformativeText(tr("Rewrite?"));
                        msgBox1.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
                        msgBox1.setButtonText(QMessageBox::Yes,tr("Yes"));
                        msgBox1.setButtonText(QMessageBox::No,tr("No"));
                        msgBox1.setButtonText(QMessageBox::Cancel,tr("Cancel"));
                        msgBox1.setDefaultButton(QMessageBox::Yes);
                        int ret2 = msgBox1.exec();
                        switch (ret2) {
                            case QMessageBox::Yes: {
                                if (file.open(QIODevice::WriteOnly)) {
                                    if (file.write(text_templates->toPlainText().toUtf8())!=text_templates->toPlainText().toUtf8().length()) {
                                        file.close();
                                        QMessageBox msgBox1(this);
                                        msgBox1.setText(tr("Unable to save the file!"));
                                        msgBox1.setIcon(QMessageBox::Warning);
                                        msgBox1.setStandardButtons(QMessageBox::Ok);
                                        msgBox1.exec();
                                    }
                                    else {
                                        file.close();                                        
                                        text_templates->clear();
                                        tek_file = "";
                                        text_templates->document()->setModified(false);
                                        text_templates->document()->setUndoRedoEnabled(false);
                                        text_templates->document()->setUndoRedoEnabled(true);
                                        tool_save->setEnabled(false);
                                        tool_undo->setEnabled(false);
                                        tool_redo->setEnabled(false);
                                        modif(false);
                                    }
                                }
                                else {
                                    QMessageBox msgBox1(this);
                                    msgBox1.setText(tr("Unable to open the file!"));
                                    msgBox1.setIcon(QMessageBox::Warning);
                                    msgBox1.setStandardButtons(QMessageBox::Ok);
                                    msgBox1.exec();
                                }
                                break;
                            }
                            case QMessageBox::No: {
                                text_templates->clear();
                                tek_file = "";
                                text_templates->document()->setModified(false);
                                text_templates->document()->setUndoRedoEnabled(false);
                                text_templates->document()->setUndoRedoEnabled(true);
                                tool_save->setEnabled(false);
                                tool_undo->setEnabled(false);
                                tool_redo->setEnabled(false);
                                modif(false);
                                break;
                            }
                            case QMessageBox::Cancel: {
                                break;
                            }
                            default: {
                                break;
                            }
                        }
                    }
                    else {
                        if (file.open(QIODevice::WriteOnly)) {
                            if (file.write(text_templates->toPlainText().toUtf8())!=text_templates->toPlainText().toUtf8().length()) {
                                QMessageBox msgBox1(this);
                                msgBox1.setText(tr("Unable to save the file!"));
                                msgBox1.setIcon(QMessageBox::Warning);
                                msgBox1.setStandardButtons(QMessageBox::Ok);
                                msgBox1.exec();
                                file.close();
                            }
                            else {
                                file.close();
                                text_templates->clear();
                                tek_file = "";
                                text_templates->document()->setModified(false);
                                text_templates->document()->setUndoRedoEnabled(false);
                                text_templates->document()->setUndoRedoEnabled(true);
                                tool_save->setEnabled(false);
                                tool_undo->setEnabled(false);
                                tool_redo->setEnabled(false);
                                modif(false);
                            }
                        }

                    }
                }

                break;
            }
            default: {
                break;
            }
        }
    }
    else {
        text_templates->clear();
        tek_file = "";
        text_templates->document()->setModified(false);
        text_templates->document()->setUndoRedoEnabled(false);
        text_templates->document()->setUndoRedoEnabled(true);
        tool_save->setEnabled(false);
        tool_undo->setEnabled(false);
        tool_redo->setEnabled(false);
        modif(false);
    }
    create_menu_open();
}

void Edit_Templates::save_file()
{
    if (tek_file.isEmpty()) {
        QString fileName = "";
        for (int i=0;;i++) {
            if (!QFile::exists(dir_file+QDir::separator()+QString("NewTemplate%1.txt").arg(i))) {
                fileName = dir_file+QDir::separator()+QString("NewTemplate%1.txt").arg(i);
                break;
            }
        }
        bool ok;
        QString text = QString::fromLocal8Bit(QInputDialog::getText(this, tr("Enter a name for the new template."),
                                                                    "", QLineEdit::Normal,QFileInfo(fileName).fileName(), &ok).toLocal8Bit());
        if (ok && !text.isEmpty()) {
            if (QFileInfo(dir_file+QDir::separator()+text).suffix().isEmpty())
                text += ".txt";
            QFile file(dir_file+QDir::separator()+text);
            if (file.exists(file.fileName())) {
                QMessageBox msgBox1;
                msgBox1.setText(tr("File is exist!"));
                msgBox1.setInformativeText(tr("Rewrite?"));
                msgBox1.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
                msgBox1.setButtonText(QMessageBox::Yes,tr("Yes"));
                msgBox1.setButtonText(QMessageBox::No,tr("No"));
                msgBox1.setButtonText(QMessageBox::Cancel,tr("Cancel"));
                msgBox1.setDefaultButton(QMessageBox::Yes);
                int ret1 = msgBox1.exec();
                switch (ret1) {
                    case QMessageBox::Yes: {
                        if (file.open(QIODevice::WriteOnly)) {
                            if (file.write(text_templates->toPlainText().toUtf8())!=text_templates->toPlainText().toUtf8().length()) {
                                file.close();
                                QMessageBox msgBox1;
                                msgBox1.setText(tr("Unable to save the file!"));
                                msgBox1.setIcon(QMessageBox::Warning);
                                msgBox1.setStandardButtons(QMessageBox::Ok);
                                msgBox1.exec();
                            }
                            else {
                                file.close();
                                tek_file = file.fileName();
                                text_templates->document()->setModified(false);
                                text_templates->document()->setUndoRedoEnabled(false);
                                text_templates->document()->setUndoRedoEnabled(true);
                                tool_save->setEnabled(false);
                                tool_undo->setEnabled(false);
                                tool_redo->setEnabled(false);
                                modif(false);
                            }
                        }
                        else {
                            QMessageBox msgBox1;
                            msgBox1.setText(tr("Unable to open the file!"));
                            msgBox1.setIcon(QMessageBox::Warning);
                            msgBox1.setStandardButtons(QMessageBox::Ok);
                            msgBox1.exec();
                        }
                        break;
                    }
                    case QMessageBox::No: {
                        break;
                    }
                    case QMessageBox::Cancel: {
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
            else {
                if (file.open(QIODevice::WriteOnly)) {
                    if (file.write(text_templates->toPlainText().toUtf8())!=text_templates->toPlainText().toUtf8().length()) {
                        QMessageBox msgBox1;
                        msgBox1.setText(tr("Unable to save the file!"));
                        msgBox1.setIcon(QMessageBox::Warning);
                        msgBox1.setStandardButtons(QMessageBox::Ok);
                        msgBox1.exec();
                        file.close();
                    }
                    else {
                        file.close();
                        tek_file = file.fileName();
                        text_templates->document()->setModified(false);
                        text_templates->document()->setUndoRedoEnabled(false);
                        text_templates->document()->setUndoRedoEnabled(true);
                        tool_save->setEnabled(false);
                        tool_undo->setEnabled(false);
                        tool_redo->setEnabled(false);
                        modif(false);
                    }
                }
            }
        }
    }
    else {
        QMessageBox msgBox;
        msgBox.setText(tr("The file is exist."));
        msgBox.setInformativeText(tr("Rewrite?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setButtonText(QMessageBox::Yes,tr("Yes"));
        msgBox.setButtonText(QMessageBox::No,tr("No"));
        msgBox.setButtonText(QMessageBox::Cancel,tr("Cancel"));
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        switch (ret) {
            case QMessageBox::Yes: {
                QFile file(tek_file);
                if (file.open(QIODevice::WriteOnly)) {
                    if (file.write(text_templates->toPlainText().toUtf8())!=text_templates->toPlainText().toUtf8().length()) {
                        file.close();
                        QMessageBox msgBox1;
                        msgBox1.setText(tr("Unable to save the file!"));
                        msgBox1.setIcon(QMessageBox::Warning);
                        msgBox1.setStandardButtons(QMessageBox::Ok);
                        msgBox1.exec();
                    }
                    else {
                        file.close();
                        text_templates->document()->setModified(false);
                        text_templates->document()->setUndoRedoEnabled(false);
                        text_templates->document()->setUndoRedoEnabled(true);
                        tool_save->setEnabled(false);
                        tool_undo->setEnabled(false);
                        tool_redo->setEnabled(false);
                        modif(false);
                    }
                }
                else {
                    QMessageBox msgBox1;
                    msgBox1.setText(tr("Unable to open the file!"));
                    msgBox1.setIcon(QMessageBox::Warning);
                    msgBox1.setStandardButtons(QMessageBox::Ok);
                    msgBox1.exec();
                }
                break;
            }
            case QMessageBox::Cancel: {
                break;
            }
            case QMessageBox::No: {
                QString fileName = "";
                for (int i=0;;i++) {
                    if (!QFile::exists(dir_file+QDir::separator()+QString("NewTemplate%1.txt").arg(i))) {
                        fileName = dir_file+QDir::separator()+QString("NewTemplate%1.txt").arg(i);
                        break;
                    }
                }
                bool ok;
                QString text = QString::fromLocal8Bit(QInputDialog::getText(this, tr("Enter a name for the new template."),
                                                                            "", QLineEdit::Normal,QFileInfo(fileName).fileName(), &ok).toLocal8Bit());
                if (ok && !text.isEmpty()) {
                    if (QFileInfo(dir_file+QDir::separator()+text).suffix().isEmpty())
                        text += ".txt";
                    QFile file(dir_file+QDir::separator()+text);
                    if (file.exists(file.fileName())) {
                        QMessageBox msgBox1;
                        msgBox1.setText(tr("File is exist!"));
                        msgBox1.setInformativeText(tr("Rewrite?"));
                        msgBox1.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
                        msgBox1.setButtonText(QMessageBox::Yes,tr("Yes"));
                        msgBox1.setButtonText(QMessageBox::No,tr("No"));
                        msgBox1.setButtonText(QMessageBox::Cancel,tr("Cancel"));
                        msgBox1.setDefaultButton(QMessageBox::Yes);
                        int ret1 = msgBox1.exec();
                        switch (ret1) {
                            case QMessageBox::Yes: {
                                if (file.open(QIODevice::WriteOnly)) {
                                    if (file.write(text_templates->toPlainText().toUtf8())!=text_templates->toPlainText().toUtf8().length()) {
                                        file.close();
                                        QMessageBox msgBox1;
                                        msgBox1.setText(tr("Unable to save the file!"));
                                        msgBox1.setIcon(QMessageBox::Warning);
                                        msgBox1.setStandardButtons(QMessageBox::Ok);
                                        msgBox1.exec();
                                    }
                                    else {
                                        file.close();
                                        tek_file = file.fileName();
                                        text_templates->document()->setModified(false);
                                        text_templates->document()->setUndoRedoEnabled(false);
                                        text_templates->document()->setUndoRedoEnabled(true);
                                        tool_save->setEnabled(false);
                                        tool_undo->setEnabled(false);
                                        tool_redo->setEnabled(false);
                                        modif(false);
                                    }
                                }
                                else {
                                    QMessageBox msgBox1;
                                    msgBox1.setText(tr("Unable to open the file!"));
                                    msgBox1.setIcon(QMessageBox::Warning);
                                    msgBox1.setStandardButtons(QMessageBox::Ok);
                                    msgBox1.exec();
                                }
                                break;
                            }
                            case QMessageBox::No: {
                                break;
                            }
                            case QMessageBox::Cancel: {
                                break;
                            }
                            default: {
                                break;
                            }
                        }
                    }
                    else {
                        if (file.open(QIODevice::WriteOnly)) {
                            if (file.write(text_templates->toPlainText().toUtf8())!=text_templates->toPlainText().toUtf8().length()) {
                                QMessageBox msgBox1;
                                msgBox1.setText(tr("Unable to save the file!"));
                                msgBox1.setIcon(QMessageBox::Warning);
                                msgBox1.setStandardButtons(QMessageBox::Ok);
                                msgBox1.exec();
                                file.close();
                            }
                            else {
                                file.close();
                                tek_file = file.fileName();
                                text_templates->document()->setModified(false);
                                text_templates->document()->setUndoRedoEnabled(false);
                                text_templates->document()->setUndoRedoEnabled(true);
                                tool_save->setEnabled(false);
                                tool_undo->setEnabled(false);
                                tool_redo->setEnabled(false);
                                modif(false);
                            }
                        }

                    }
                }
                break;
            }
            default: {
                break;
            }
        }
    }
    create_menu_open();
}

void Edit_Templates::delete_file()
{
    if (tek_file.isEmpty()) {
        text_templates->clear();
        tek_file = "";
        text_templates->document()->setModified(false);
        text_templates->document()->setUndoRedoEnabled(false);
        text_templates->document()->setUndoRedoEnabled(true);
        tool_save->setEnabled(false);
        tool_undo->setEnabled(false);
        tool_redo->setEnabled(false);
        modif(false);
    }
    else {
        QMessageBox msgBox1;
        msgBox1.setText(tr("Are you sure you want to delete a file?"));
        msgBox1.setIcon(QMessageBox::Question);
        msgBox1.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox1.setButtonText(QMessageBox::Yes,tr("Yes"));
        msgBox1.setButtonText(QMessageBox::No,tr("No"));
        int ret=msgBox1.exec();
        switch (ret) {
            case QMessageBox::Yes: {
                if (!QFile::remove(tek_file)) {
                    QMessageBox msgBox2;
                    msgBox2.setText(tr("Unable to delete a file!"));
                    msgBox2.setIcon(QMessageBox::Warning);
                    msgBox2.setStandardButtons(QMessageBox::Ok);
                    msgBox2.exec();
                }
                else {                    
                    text_templates->clear();
                    tek_file = "";
                    text_templates->document()->setModified(false);
                    text_templates->document()->setUndoRedoEnabled(false);
                    text_templates->document()->setUndoRedoEnabled(true);
                    tool_save->setEnabled(false);
                    tool_undo->setEnabled(false);
                    tool_redo->setEnabled(false);
                    modif(false);
                }
                break;
            }
            case QMessageBox::No: {
                break;
            }
            default: {
                break;
            }
        }
    }
    create_menu_open();
}

void Edit_Templates::closeEvent(QCloseEvent *event)
{
    tool_save->click();
    event->accept();
}

void Edit_Templates::open_help()
{
    QDesktopServices::openUrl(QUrl("http://wiki.soulruins.info/filmonaizer/help/start"));
}
