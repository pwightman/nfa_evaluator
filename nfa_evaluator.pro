#-------------------------------------------------
#
# Project created by QtCreator 2011-11-18T14:52:37
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = nfa_evaluator
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += debug

mac {
  CONFIG -= app_bundle
}

TEMPLATE = app

SOURCES += main.cpp \
    traversal.cpp \
    nfa.cpp \
    node.cpp

HEADERS += \
    traversal.h\
    nfa.h \
    node.h

QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS *= -fopenmp
