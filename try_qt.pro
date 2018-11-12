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
    libraries/src/contraction.cpp \
    librariessrc/einsum.cpp \
    libraries/src/frequencies_group.cpp \
    libraries/src/grid_rect_2D.cpp \
    libraries/src/ProfileCpu.cpp \
    libraries/src/receivers_rect_2D.cpp \
    libraries/src/sources_rect_2D.cpp \
    libraries/src/volComplexField_rect_2D_cpu.cpp \
    libraries/src/volField_rect_2D_cpu.cpp \

DISTFILES += \
    libraries/src/chi_est_sine.txt \
    libraries/src/chi.txt \
    libraries/src/p_data.txt

HEADERS += \
    libraries/include/read_input_fwi_into_vec.h \
    libraries/include/calcField.h \
    libraries/include/contraction.h \
    libraries/include/einsum.h \
    libraries/include/frequencies_group.h \
    libraries/include/greens_rect_2D_cpu.h \
    libraries/include/GreensFunctions.h \
    libraries/include/grid_rect_2D.h \
    libraries/include/inversion.h \
    libraries/include/helper.h \
    libraries/include/ProfileCpu.h \
    libraries/include/ProfileInterface.h \
    libraries/include/receivers_rect_2D.h \
    libraries/include/sources_rect_2D.h \
    libraries/include/volComplexField_rect_2D_cpu.h \
    libraries/include/volComplexField_rect_2D.h \
    libraries/include/volField_rect_2D_cpu.h \
    libraries/include/volField_rect_2D.h \
    libraries/include/input_parameters.h \
    libraries/include/inversion_cpu.h \
    libraries/include/chi_visualisation_in_integer_form.h \
    libraries/include/create_csv_files_for_chi.h

