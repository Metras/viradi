# -------------------------------------------------
# Project created by QtCreator 2010-07-15T15:11:06
# -------------------------------------------------
TARGET = SpecPlot
TEMPLATE = app
QT += multimedia
SOURCES += main.cpp \
    mainwindow.cpp \
    canvaspicker.cpp \
    wavfile.cpp \
    specplot.cpp \
    specbox.cpp
HEADERS += mainwindow.h \
    specplot.h \
    specplot.h \
    wavfile.h \
    specbox.h \
    canvaspicker.h
LIBS += /usr/lib/libqwt.so.5
LIBS += /usr/lib/libfftw3.a
LIBS += /usr/lib/libfftw3f.a
LIBS += /usr/lib/libfftw3l.a
