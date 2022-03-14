#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QSplitter>
#include "StudioCore/Serial/serial.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QNodeTreeView;
class QWorldsView;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int getInt();

private slots:
    void onClick();
    void onTimer();

    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void onFileQuit();

private:

    void createActions();
    void createMenus();

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    QSplitter *splitter1;
    QSplitter *splitter2;
    QSplitter* splitter3;

    QNodeTreeView* node_treeview;
    QWorldsView* worlds_view;

    Ui::MainWindow *ui;
    QPushButton *button;
    QTextEdit *text_edit;
    serial::Serial *my_serial;
    QTimer *timer;
};
#endif // MAINWINDOW_H
