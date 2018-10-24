TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += qt

QMAKE_CXX = mpicxx
QMAKE_CXX_RELEASE = $$QMAKE_CXX
QMAKE_CXX_DEBUG = $$QMAKE_CXX
QMAKE_LINK = $$QMAKE_CXX
QMAKE_CC = mpicc

#QMAKE_CFLAGS += $$system(mpicc --showme:compile)
QMAKE_CFLAGS += $$system(mpicc -compile-info)
#QMAKE_LFLAGS += $$system(mpicxx --showme:link)
#QMAKE_LFLAGS += $$system(mpicxx -link-info)
#QMAKE_CXXFLAGS += $$system(mpicxx --showme:compile) -DMPICH_IGNORE_CXX_SEEK
#QMAKE_CXXFLAGS_RELEASE += $$system(mpicxx --showme:compile) -DMPICH_IGNORE_CXX_SEEK
#QMAKE_CXXFLAGS += $$system(mpicxx -compile-info) -DMPICH_IGNORE_CXX_SEEK
#QMAKE_CXXFLAGS_RELEASE += $$system(mpicxx -compile-info) -DMPICH_IGNORE_CXX_SEEK

QMAKE_CXXFLAGS_RELEASE = -O3 -march=native -ftree-loop-distribution -malign-double
QMAKE_CFLAGS_RELEASE -= -O2 -march=native

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/src
INCLUDEPATH += /usr/include/python2.7
LIBS += -lpython2.7
LIBS += -lOpenCL

SOURCES += main.cpp \
    src/contraction.cpp \
    src/einsum.cpp \
    src/frequencies_group.cpp \
    src/grid_rect_2D.cpp \
    src/ProfileCpu.cpp \
    src/receivers_rect_2D.cpp \
    src/sources_rect_2D.cpp \
    src/volComplexField_rect_2D_cpu.cpp \
    src/volField_rect_2D_cpu.cpp \

DISTFILES += \
    src/chi_est_sine.txt \
    src/chi.txt \
    src/p_data.txt

HEADERS += \
    include/read_input_fwi_into_vec.h \
    include/calcField.h \
    include/contraction.h \
    include/einsum.h \
    include/frequencies_group.h \
    include/greens_rect_2D_cpu.h \
    include/GreensFunctions.h \
    include/grid_rect_2D.h \
    include/grid.h \
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
    include/input_parameters.h \
    include/inversion_cpu.h \ 
    include/chi_visualisation_in_integer_form.h \
    include/create_csv_files_for_chi.h

