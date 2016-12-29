TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += qt
QMAKE_CXXFLAGS_RELEASE = -O3 -march=native -ftree-loop-distribution -malign-double
QMAKE_CFLAGS_RELEASE -= -O2 -march=native
QMAKE_CXXFLAGS_RELEASE -= -g -Wall

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/src
INCLUDEPATH += /home/amittal/Eigen
INCLUDEPATH += /usr/include/python2.7
LIBS += -lpython2.7


SOURCES += main.cpp \
    src/contraction.cpp \
    src/einsum.cpp \
    src/frequencies.cpp \
    src/grid_rect_2D.cpp \
    src/ProfileCpu.cpp \
    src/receivers_rect_2D.cpp \
    src/sources_rect_2D.cpp \
    src/volComplexField_rect_2D_cpu.cpp \
    src/volField_rect_2D_cpu.cpp

DISTFILES += \
    src/chi_est_sine.txt \
    src/chi.txt \
    src/p_data.txt

HEADERS += \
    include/calcField.h \
    include/contraction.h \
    include/einsum.h \
    include/frequencies.h \
    include/greens_rect_2D_cpu.h \
    include/GreensFunctions.h \
    include/grid_rect_2D.h \
    include/grid.h \
    include/helper.h \
    include/inversion.h \
    include/ProfileCpu.h \
    include/ProfileInterface.h \
    include/receivers_rect_2D.h \
    include/sources_rect_2D.h \
    include/tests_helper.h \
    include/volComplexField_rect_2D_cpu.h \
    include/volComplexField_rect_2D.h \
    include/volField_rect_2D_cpu.h \
    include/volField_rect_2D.h \
    include/input_parameters.h

