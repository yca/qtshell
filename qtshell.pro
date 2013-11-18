#-------------------------------------------------
#
# Project created by QtCreator 2013-11-17T22:52:57
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = qtshell
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    compiler.cpp \
    parser.cpp

HEADERS += \
    compiler.h \
    parser.h

target.path = /usr/local/bin
INSTALLS += target
