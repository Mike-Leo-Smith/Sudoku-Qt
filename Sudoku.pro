#-------------------------------------------------
#
# Project created by QtCreator 2017-08-29T19:45:30
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sudoku
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp \
    Engine/Sudoku.cpp \
    Engine/SudokuCreator.cpp \
    Engine/SudokuSolver.cpp \
    UI/Window.cpp \
    UI/SudokuView.cpp \
    UI/SudokuController.cpp \
    UI/BannerView.cpp \
    UI/KeyboardController.cpp \
    UI/StopwatchController.cpp

HEADERS += \
    Engine/Sudoku.h \
    Engine/SudokuCreator.h \
    Engine/SudokuSolver.h \
    UI/Window.h \
    UI/SudokuView.h \
    UI/SudokuController.h \
    UI/GameState.h \
    UI/BannerView.h \
    UI/KeyboardController.h \
    UI/StopwatchController.h

FORMS += \
    UI/Window.ui

RESOURCES += \
    Assets/assets.qrc

