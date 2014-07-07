#-------------------------------------------------
#
# Project created by QtCreator 2013-09-28T18:22:25
#
#-------------------------------------------------

QT       += core gui xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = spriteeditorplugin
TEMPLATE = lib
CONFIG += plugin

DEFINES += SPRITEEDITORPLUGIN_LIBRARY \
    ATHENA_USE_QT

include(../Athena/Athena.pri)

INCLUDEPATH += \
    include \
    ../PluginFramework/include \
    ../Athena/include \
    ../Updater/include

DESTDIR = $$OUT_PWD/../../build/plugins
UI_DIR = ui

QMAKE_CXXFLAGS = -std=c++11

LIBS += \
    -L$$OUT_PWD/../PluginFramework -lpluginframework \
    -L$$OUT_PWD/../Updater -lupdater

CONFIG(release, release|debug){
    DEFINES -= SS_DEBUG
    # We don't want the objects, or MOC sources
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/release
    MOC_DIR = moc/release
}


CONFIG(debug, debug|release){
    DEFINES += SS_DEBUG
    # We don't want the objects, or MOC sources
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/debug
    MOC_DIR = moc/debug
}


SOURCES += \
    src/SpriteEditorPlugin.cpp \
    src/SpriteDocument.cpp \
    src/SpriteInfo.cpp \
    src/SpriteEditorFrame.cpp \
    src/SpriteCanvas.cpp \
    src/NameTreeItem.cpp \
    src/ObjectController.cpp \
    src/GraphicsScene.cpp \
    src/PartGraphicsItem.cpp

HEADERS += \
    include/SpriteEditorPlugin.hpp\
    include/spriteeditorplugin_global.hpp \
    include/SpriteDocument.hpp \
    include/SpriteInfo.hpp \
    include/SpriteEditorFrame.hpp \
    include/SpriteCanvas.hpp \
    include/NameTreeItem.hpp \
    include/ObjectController.hpp \
    include/GraphicsScene.hpp \
    include/PartGraphicsItem.hpp

OTHER_FILES += \
    spriteeditor.json

FORMS += \
    ui/SpriteInfo.ui \
    ui/SpriteEditorFrame.ui \
    ui/SpriteTree.ui \
    ui/SpriteCanvas.ui

include(../qtpropertybrowser/src/qtpropertybrowser.pri);

