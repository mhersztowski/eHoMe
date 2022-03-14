#include "mainwindow.h"
#include "EHLangMainWindow.h"
#include "EHClientServerMainWindow.h"
#include "Core/Lang/PascalP/PascalPTest.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    //MainWindow w;
    //w.show();

    //EHLangMainWindow w;
    //w.show();

    //EHClientServerMainWindow w;
    //w.show();

    PascalPTest::TestAll();

    return a.exec();
}
