#include "EHLangMainWindow.h"
#include <QMenuBar>

#include "StudioGui/View/QEHLangTextView.h"
#include "StudioGui/View/QEHLangErrorsList.h"

EHLangMainWindow::EHLangMainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    createActions();
    createMenus();

    splitter1 = new QSplitter(this);
    splitter2 = new QSplitter(this);
    this->setCentralWidget(splitter1);

    text_edit = new QEHLangTextView();
    errors_list = new QEHLangErrorsList();

    QTextEdit *textedit1 = new QTextEdit();
    //QTextEdit *textedit2 = new QTextEdit;
    //QTextEdit *textedit3 = new QTextEdit;

    //node_treeview = new QNodeTreeView();

    splitter1->addWidget(text_edit);
    splitter1->addWidget(errors_list);

    /*
    splitter1->addWidget(splitter2);
    splitter2->setOrientation(Qt::Orientation::Vertical);
    splitter2->addWidget(node_treeview);
    splitter2->addWidget(textedit3);

    */

}

EHLangMainWindow::~EHLangMainWindow() {

}



void EHLangMainWindow::createActions() {

    buildAct = new QAction(tr("Build"), this);
    //buildAct->setShortcuts(QKeySequence::New);
    //buildAct->setStatusTip(tr("Create a new file"));
    connect(buildAct, &QAction::triggered, this, &EHLangMainWindow::onProjectBuild);

    runAct = new QAction(tr("Run"), this);
    //runAct->setShortcuts(QKeySequence::New);
    //runAct->setStatusTip(tr("Create a new file"));
    connect(runAct, &QAction::triggered, this, &EHLangMainWindow::onDebugRun);

}

void EHLangMainWindow::createMenus() {

    fileMenu = menuBar()->addMenu(tr("&File"));

    projectMenu = menuBar()->addMenu(tr("&Project"));
    projectMenu->addAction(buildAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));

}

void EHLangMainWindow::onProjectBuild(){

    eh_lang = new EHLang::EHLang();

    QString str = text_edit->toPlainText();
    str.replace("\n", "\r\n");
    str.replace("\r\r\n", "\r\n");

    QByteArray inUtf8 = str.toUtf8();
    const char *data = inUtf8.constData();

    eh_lang->initFromUtf8Buffer(data, inUtf8.size());
    eh_lang->parse();
    errors_list->update(eh_lang);
}

void EHLangMainWindow::onDebugRun() {


}
