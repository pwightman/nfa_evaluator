#-------------------------------------------------
#
# Project created by QtCreator 2011-11-18T14:52:37
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = test_nfa
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += debug

mac {
  CONFIG -= app_bundle
}

TEMPLATE = app

SOURCES += test_nfa.cpp \
    node.cpp \
    nfa.cpp

HEADERS += \
    node.h \
    nfa.h

LIBS += lex.yy.o

QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS *= -fopenmp

LEXSOURCES += reparse.l
YACCSOURCES += reparse.y

QMAKE_LEX = flex
QMAKE_YACC = bison

QMAKE_YACCFLAGS = -v -d
