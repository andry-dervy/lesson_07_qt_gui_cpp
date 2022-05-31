QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS += QtLanguage_ru.ts QtLanguage_en.ts
CODECFORSRC = UTF-8

SOURCES += \
    documentview.cpp \
    drawgraphtoolbarsingleton.cpp \
    keypresseventfilter.cpp \
    languagevisitor.cpp \
    main.cpp \
    mainwindow.cpp \
    textformatetoolbarsingleton.cpp \
    widgetsettings.cpp \
    widgettreedirs.cpp \
    toolbarelementsfactory.cpp

HEADERS += \
    documentview.h \
    drawgraphtoolbarsingleton.h \
    keypresseventfilter.h \
    languagevisitor.h \
    mainwindow.h \
    textformatetoolbarsingleton.h \
    widgetsettings.h \
    widgettreedirs.h \
    toolbarelementsfactory.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    help.qrc \
    icons.qrc \
    language.qrc \
    styles.qrc
