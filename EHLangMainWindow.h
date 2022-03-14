#ifndef EHLANGMAINWINDOW_H
#define EHLANGMAINWINDOW_H

#include <QSplitter>
#include <QMainWindow>
#include "StudioCore/EHLang/EHLang.h"

class QEHLangTextView;
class QEHLangErrorsList;

class EHLangMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    EHLangMainWindow(QWidget *parent = nullptr);
    ~EHLangMainWindow();

private slots:

    void onProjectBuild();
    void onDebugRun();

private:

    void createActions();
    void createMenus();

    QAction *buildAct;
    QAction *runAct;

    QAction *saveAct;
    QAction *exitAct;

    QMenu *fileMenu;
    QMenu *projectMenu;
    QMenu *helpMenu;

    QSplitter *splitter1;
    QSplitter *splitter2;

    QEHLangTextView* text_edit;
    QEHLangErrorsList* errors_list;

    EHLang::EHLang* eh_lang;

    QTimer *timer;
};

#endif // EHLANGMAINWINDOW_H
