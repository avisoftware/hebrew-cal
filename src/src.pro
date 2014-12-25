#-------------------------------------------------
#
# Project created by QtCreator 2013-08-05T17:21:35
#
#-------------------------------------------------


QT += widgets
QT += sql
!isEmpty(QT.printsupport.name): QT += printsupport



TARGET = hebrew-cal
TEMPLATE = app
unix {
  #VARIABLES
  isEmpty(PREFIX) {
    PREFIX = /usr
  }
  BINDIR = $$PREFIX/bin
  DATADIR =$$PREFIX/share

  DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

  #MAKE INSTALL

  INSTALLS += target desktop icon128

  target.path =$$BINDIR

  desktop.path = $$DATADIR/applications/
  desktop.files += ../data/$${TARGET}.desktop

  icon128.path = $$DATADIR/pixmaps/
  icon128.files += ../data/$${TARGET}_128.png
}

SOURCES += main.cpp\
        mainwindow.cpp \
    calWidget/calwidget.cpp \
    hebD.cpp \
    hebtimes.cpp\
    hebstrings.cpp \
    jumptodatedialog.cpp \
    omerdialog.cpp \
    settingsdialog.cpp \
    locationsview.cpp \
    mytableview.cpp \
    locationeditdialog.cpp \
    settingsfunc.cpp \
    noteplugin.cpp \
    functions.cpp \
    notewindow.cpp \
    eventlist.cpp \
    dialogprinttables.cpp

HEADERS  += mainwindow.h \
    calWidget/calwidget.h \
    hebD.h \
    hebtimes.h\
    jumptodatedialog.h \
    omerdialog.h \
    settingsdialog.h \
    locationsview.h \
    mytableview.h \
    locationeditdialog.h \
    settingsfunc.h \
    noteplugin.h \
    functions.h \
    notewindow.h \
    eventlist.h \
    dialogprinttables.h

FORMS    += mainwindow.ui \
    calWidget/calwidget.ui \
    dialogjumptodate.ui \
    omerdialog.ui \
    settingsdialog.ui \
    locationsview.ui \
    locationeditdialog.ui \
    notewindow.ui \
    dialogprinttables.ui \
    about.ui

OTHER_FILES +=

RESOURCES += \
    rc.qrc


