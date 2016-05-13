#-------------------------------------------------
#
# Project created by QtCreator 2015-03-05T00:45:33
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MultiInput
TEMPLATE = app


SOURCES += main.cpp\
        multiinput.cpp

HEADERS  += multiinput.h

FORMS    += multiinput.ui

CONFIG += c++11 static
QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++ -mmacosx-version-min=10.7
#QMAKE_LFLAGS += -Xlinker
LIBS += -stdlib=libc++ -mmacosx-version-min=10.7
