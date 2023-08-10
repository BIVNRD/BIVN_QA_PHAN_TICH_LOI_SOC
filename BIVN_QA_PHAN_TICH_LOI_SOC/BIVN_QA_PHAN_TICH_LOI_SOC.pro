QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



#INCLUDEPATH += E:\QA_PRINT_ERROR_DETECTOR\opencv\include
#LIBS += E:\QA_PRINT_ERROR_DETECTOR\opencv\lib\*
INCLUDEPATH += D:\BIVN\BIVN_QA_PHAN_TICH_LOI_SOC\BIVN_QA_PHAN_TICH_LOI_SOC\opencv\include
LIBS += D:\BIVN\BIVN_QA_PHAN_TICH_LOI_SOC\BIVN_QA_PHAN_TICH_LOI_SOC\opencv\lib\*


SOURCES += \
    background/background.cpp \
    main.cpp \
    mainwindow.cpp \
    models/checkarea.cpp \
    models/paper.cpp \
    models/printerror.cpp \
    processor/processor.cpp \
    processor/temp_process.cpp

HEADERS += \
    background/background.h \
    mainwindow.h \
    models/checkarea.h \
    models/paper.h \
    models/printerror.h \
    processor/processor.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
