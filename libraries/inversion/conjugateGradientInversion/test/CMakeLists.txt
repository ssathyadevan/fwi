# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CONJUGATE_GRADIENT_INVERSION} )
# Might have to include core library as well.
include_directories(../include)
include_directories( ${CMAKE_SOURCE_DIR}/libraries/io/include )
include_directories( ${CMAKE_SOURCE_DIR}/libraries/utilities/include)

#Build the tests!
build_test(TARGET ConjugateGradientInversionInputCardReaderTest SOURCE ConjugateGradientInversionInputCardReaderTest.cpp LIBRARIES conjugate_gradient_inversion_library io_library)
