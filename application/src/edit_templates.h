#ifndef EDIT_TEMPLATES_H
#define EDIT_TEMPLATES_H

#include <QMainWindow>
#include <QApplication>
#include <QMenu>
#include <QFile>
#include <QSyntaxHighlighter>
#include <QToolBar>
#include <QToolButton>
#include <QPlainTextEdit>

#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>

#include <QDir>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat tagsFormat;
    QTextCharFormat globaltagsFormat;
};


class Edit_Templates : public QMainWindow
{
    Q_OBJECT
public:
    Edit_Templates(QString dir, QString file_templates, QList<QString> tags_plug_search, QList<QString> tags_plug_mediainfo, QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *event);

private:
    void create_menu_open();
    QMenu *menu_open;
    QMenu *menu_tags;
	QMenu *menu_local_variables;
	QMenu *menu_global_variables;
	QMenu *menu_mediainfo_variables;
    QMenu *menu_symbol;

    QToolBar *bar_file;
    QToolBar *bar_copypaste;
    QToolBar *bar_undoredo;
    QToolBar *bar_paste_;
    QToolButton *tool_new;
    QToolButton *tool_open;
    QToolButton *tool_save;
    QToolButton *tool_delete_file;
    QToolButton *tool_cut;
    QToolButton *tool_copy;
    QToolButton *tool_paste;
    QToolButton *tool_undo;
    QToolButton *tool_redo;
    QToolButton *tool_paste_tags;
    QToolButton *tool_paste_symbol;
    QToolButton *tool_help;
    QPlainTextEdit *text_templates;

    QList<QString> tt;
    Highlighter *highlighter;

    QString tek_file;
    QString dir_file;

private slots:
    void _triggered_open(QAction* ret);
    void _triggered_tags(QAction* ret);
    void _triggered_symbol(QAction* ret);
    void modif(bool b);
    void new_file();
    void save_file();
    void delete_file();
    void open_help();
};

#endif // EDIT_TEMPLATES_H
