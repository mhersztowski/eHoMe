#ifndef EHCLIENTSERVERMAINWINDOW_H
#define EHCLIENTSERVERMAINWINDOW_H

#include <QSplitter>
#include <QMainWindow>


class EHClientServerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    EHClientServerMainWindow(QWidget *parent = nullptr);
    ~EHClientServerMainWindow();

private slots:

    void onClientSwitch();
    void onDebugRun();
    void onUpdateTimer();

private:

    void createActions();
    void createMenus();

    QAction *clientSwitchAction;
    QAction *runAct;

    QAction *saveAct;
    QAction *exitAct;

    QMenu *fileMenu;
    QMenu *clientMenu;
    QMenu *helpMenu;

    QSplitter *splitter1;
    QSplitter *splitter2;

    QTimer *timer;
};

#endif // EHCLIENTSERVERMAINWINDOW_H
