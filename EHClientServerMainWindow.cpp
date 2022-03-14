#include "EHClientServerMainWindow.h"
#include <QMenuBar>
#include <QTextEdit>
#include <QTimer>
#include "Core/Server/Server.h"
#include "Core/Client/Client.h"


EHClientServerMainWindow::EHClientServerMainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    createActions();
    createMenus();

    splitter1 = new QSplitter(this);
    splitter2 = new QSplitter(this);
    this->setCentralWidget(splitter1);

    QTextEdit* textedit1 = new QTextEdit;
    QTextEdit *textedit2 = new QTextEdit;
    QTextEdit *textedit3 = new QTextEdit;

    splitter1->addWidget(textedit1);

    splitter1->addWidget(splitter2);
    splitter2->setOrientation(Qt::Orientation::Vertical);
    splitter2->addWidget(textedit2);
    splitter2->addWidget(textedit3);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &EHClientServerMainWindow::onUpdateTimer);
    timer->start(20);

    Client::get()->init();
    Server::get()->init();

}

EHClientServerMainWindow::~EHClientServerMainWindow() {

}



void EHClientServerMainWindow::createActions() {

    clientSwitchAction = new QAction(tr("Switch"), this);
    //buildAct->setShortcuts(QKeySequence::New);
    //buildAct->setStatusTip(tr("Create a new file"));
    connect(clientSwitchAction, &QAction::triggered, this, &EHClientServerMainWindow::onClientSwitch);

    runAct = new QAction(tr("Run"), this);
    //runAct->setShortcuts(QKeySequence::New);
    //runAct->setStatusTip(tr("Create a new file"));
    connect(runAct, &QAction::triggered, this, &EHClientServerMainWindow::onDebugRun);

}

void EHClientServerMainWindow::createMenus() {

    fileMenu = menuBar()->addMenu(tr("&File"));

    clientMenu = menuBar()->addMenu(tr("Client"));
    clientMenu->addAction(clientSwitchAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));

}

void EHClientServerMainWindow::onUpdateTimer() {
    Vector<Package*>& clientSendPackages = Client::get()->getPackageSendList();
    Vector<Package*>& serverReceivePackages = Server::get()->getPackageReceiveList();

    for (int i = 0; i < clientSendPackages.size(); i++) {
        if (clientSendPackages[i]->getTo() == "master")
        {
            serverReceivePackages.append(clientSendPackages[i]);
        }
        else {
            delete clientSendPackages[i];
        }
    }
    clientSendPackages.clear();

    Vector<Package*>& serverSendPackages = Server::get()->getPackageSendList();
    Vector<Package*>& clientReceivePackages = Client::get()->getPackageReceiveList();

    for (int i = 0; i < serverSendPackages.size(); i++)
    {
        if (serverSendPackages[i]->getTo() == "" || serverSendPackages[i]->getTo() == Client::get()->getName()) {
            clientReceivePackages.append(serverSendPackages[i]);
        }
        else {
            delete serverSendPackages[i];
        }
    }
    serverSendPackages.clear();


    Client::get()->update();
    Server::get()->update();
}

void EHClientServerMainWindow::onClientSwitch(){

}

void EHClientServerMainWindow::onDebugRun() {


}
