cmake_minimum_required(VERSION 3.5)

project(eHoMeStudio VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
		EHLangMainWindow.cpp
		EHLangMainWindow.h
		EHClientServerMainWindow.cpp
		EHClientServerMainWindow.h
		Core/Types/Base.h
		Core/Types/Node.h
		Core/Types/Node.cpp
		Core/Types/NodeType.h
		Core/Types/NodeType.cpp
		Core/Types/NodeTypeList.h
		Core/Types/NodeTypeList.cpp
		Core/Types/NodeFactory.h
		Core/Types/NodeFactory.cpp
		Core/Types/Property.h
		Core/Types/Property.cpp
		Core/Types/PropertyType.h
		Core/Types/PropertyType.cpp
		Core/Types/PropertySet.h
		Core/Types/PropertySet.cpp
		Core/Types/Str.h
		Core/Types/Str.cpp
		Core/Types/Vector.h
		Core/Utils/Buffer.h
		Core/Utils/Buffer.cpp
		Core/Utils/Singleton.h
		Core/Utils/Stream.h
		Core/Utils/Stream.cpp
		Core/Utils/Package.h
		Core/Utils/Package.cpp
		Core/Utils/Route.h
		Core/Utils/Route.cpp
		Core/World/Simulation.h
		Core/World/Simulation.cpp
		Core/World/TestNode.h
		Core/World/TestNode.cpp
		Core/World/World.h
		Core/World/World.cpp
		Core/World/Worlds.h
		Core/World/Worlds.cpp
		Core/Client/Client.h
		Core/Client/Client.cpp
		Core/Client/ClientInit.h
		Core/Client/ClientInit.cpp
		Core/Server/Server.h
		Core/Server/Server.cpp
		Core/Server/ServerInit.h
		Core/Server/ServerInit.cpp
		Core/Lang/PascalP/PascalPUtils.h
		Core/Lang/PascalP/PascalPUtils.cpp
		Core/Lang/PascalP/PascalPVM.h
		Core/Lang/PascalP/PascalPVM.cpp
		Core/Lang/PascalP/PascalPLex.h
		Core/Lang/PascalP/PascalPLex.cpp
		Core/Lang/PascalP/PascalPParser.h
		Core/Lang/PascalP/PascalPParser.cpp
		Core/Lang/PascalP/PascalPSymTab.h
		Core/Lang/PascalP/PascalPSymTab.cpp
		Core/Lang/PascalP/PascalPTest.h
		Core/Lang/PascalP/PascalPTest.cpp
		StudioCore/EHLang/CodeGenerator.h
		StudioCore/EHLang/EHLang.h
		StudioCore/EHLang/Logic.h
		StudioCore/EHLang/Logic.cpp
		StudioCore/EHLang/Parser.h
		StudioCore/EHLang/Parser.cpp
		StudioCore/EHLang/Scanner.h
		StudioCore/EHLang/Scanner.cpp
		StudioCore/EHLang/SymbolTable.h
		StudioCore/EHLang/SymbolTable.cpp
		StudioCore/Serial/list_ports_win.cc
		StudioCore/Serial/serial.h
		StudioCore/Serial/serial.cc
		StudioCore/Serial/win.h
		StudioCore/Serial/win.cc
		StudioGui/View/QEHLangErrorsList.h
		StudioGui/View/QEHLangErrorsList.cpp
		StudioGui/View/QEHLangTextView.h
		StudioGui/View/QEHLangTextView.cpp
		StudioGui/View/QNodeTreeView.h
		StudioGui/View/QNodeTreeView.cpp
		StudioGui/View/QWorldsView.h
		StudioGui/View/QWorldsView.cpp
		StudioGui/Widgets/QClientsWidget.h
		StudioGui/Widgets/QClientsWidget.cpp
		StudioGui/Widgets/QPropertyListWidget.h
		StudioGui/Widgets/QPropertyListWidget.cpp
		StudioGui/Widgets/QWorldWidget.h
		StudioGui/Widgets/QWorldWidget.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_SOURCES})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(eHoMeStudio
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(eHoMeStudio SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(eHoMeStudio
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(eHoMeStudio PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

TARGET_LINK_LIBRARIES(eHoMeStudio PRIVATE SetupAPI)
TARGET_LINK_LIBRARIES(eHoMeStudio PRIVATE AdvAPI32)

set_target_properties(eHoMeStudio PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(eHoMeStudio)
endif()
