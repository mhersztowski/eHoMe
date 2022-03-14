#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTimer>
#include <QListView>
#include <QTreeView>
#include "StudioGui/View/QNodeTreeView.h"
#include "StudioGui/View/QWorldsView.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*
    button = new QPushButton("Polacz", this);
    button->move(10, 10);

    connect(button, SIGNAL(clicked()), this, SLOT(onClick()));

    text_edit = new QTextEdit(this);
    text_edit->move(10, 50);
    text_edit->resize(500, 500);
    */

    //my_serial = new serial::Serial("COM3", 115200, serial::Timeout::simpleTimeout(10));
    /*
    my_serial->setPort("COM3");
    my_serial->setBaudrate(115200);

    my_serial->setBytesize(serial::eightbits);
    my_serial->setStopbits(serial::stopbits_one);
    my_serial->setParity(serial::parity_none);
    my_serial->setFlowcontrol(serial::flowcontrol_none);
    my_serial->open();
    */
    //my_serial->setTimeout(serial::Timeout::simpleTimeout(10));

    //my_serial->setRTS(true);
    //my_serial->setDTR(true);


    //

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimer);
    timer->start(1000);
    createActions();
    createMenus();

    splitter1 = new QSplitter(this);
    splitter2 = new QSplitter(this);
    this->setCentralWidget(splitter1);
    QTextEdit *textedit1 = new QTextEdit;
    //QTextEdit *textedit2 = new QTextEdit;
    QTextEdit *textedit3 = new QTextEdit;

    node_treeview = new QNodeTreeView();
    worlds_view = new QWorldsView();


    splitter1->addWidget(textedit1);
    splitter1->addWidget(splitter2);
    splitter2->setOrientation(Qt::Orientation::Vertical);
    splitter2->addWidget(worlds_view);
    splitter2->addWidget(node_treeview);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions() {
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::onFileNew);

    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    //openAct->setStatusTip(tr("Create a new file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::onFileOpen);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    //openAct->setStatusTip(tr("Create a new file"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::onFileSave);

    exitAct = new QAction(tr("&Quit"), this);
    //exitAct->setShortcuts(QKeySequence::);
    //openAct->setStatusTip(tr("Create a new file"));
    connect(exitAct, &QAction::triggered, this, &MainWindow::onFileQuit);
}

void MainWindow::createMenus() {

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));

    helpMenu = menuBar()->addMenu(tr("&Help"));
}

void MainWindow::onFileNew() {

}

void MainWindow::onFileOpen() {

}

void MainWindow::onFileSave() {

}

void MainWindow::onFileQuit() {

}

int MainWindow::getInt() {
    return 0;
}

void MainWindow::onTimer() {

    /*
    if (my_serial->isOpen()) {
        size_t bytes_wrote = my_serial->write("Test\n");

        uint8_t  buffer[100];
        size_t bytes_read = my_serial->read(buffer, 16);
        buffer[bytes_read] = 0;
        QString str((char*)buffer);
        text_edit->append(str);

        text_edit->append(QString::number(bytes_read));
    }
    */

}

void MainWindow::onClick() {
    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
}
