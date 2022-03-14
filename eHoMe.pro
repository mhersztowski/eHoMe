QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Core/Types/Property.cpp \
    Core/Types/PropertySet.cpp \
    Core/World/TestNode.cpp \
    EHLangMainWindow.cpp \
    StudioCore/EHLang/Parser.cpp \
    StudioCore/EHLang/Scanner.cpp \
    StudioCore/EHLang/SymbolTable.cpp \
    StudioGui/View/QEHLangErrorsList.cpp \
    StudioGui/View/QEHLangTextView.cpp \
    StudioGui/View/QNodeTreeView.cpp \
    main.cpp \
    mainwindow.cpp \
	Core/Types/Node.cpp \
	Core/Types/Str.cpp \
	Core/Utils/Buffer.cpp \
	Core/Utils/Stream.cpp \
    StudioCore/Serial/list_ports_win.cc \
    StudioCore/Serial/serial.cc \
    StudioCore/Serial/win.cc

HEADERS += \
    Core/Types/Base.h \
    Core/Types/Property.h \
    Core/Types/PropertySet.h \
    Core/Types/Vector.h \
    Core/Utils/Singleton.h \
    Core/World/TestNode.h \
    EHLangMainWindow.h \
    StudioCore/EHLang/CodeGenerator.h \
    StudioCore/EHLang/EHLang.h \
    StudioCore/EHLang/Parser.h \
    StudioCore/EHLang/Scanner.h \
    StudioCore/EHLang/SymbolTable.h \
    StudioGui/View/QEHLangErrorsList.h \
    StudioGui/View/QEHLangTextView.h \
    StudioGui/View/QNodeTreeView.h \
    mainwindow.h \
	Core/Types/Node.h \
	Core/Types/Str.h \
	Core/Utils/Buffer.h \
	Core/Utils/Stream.h \
    StudioCore/Serial/serial.h \
    StudioCore/Serial/v8stdint.h \
    StudioCore/Serial/win.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -lSetupAPI

unix|win32: LIBS += -lAdvAPI32
