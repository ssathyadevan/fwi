file(GLOB CPP_SOURCES *.cpp)
file(GLOB H_HEADERS ../include/*.h)

add_library(mpi_conjugate_gradient_inversion_library ${CPP_SOURCES} ${H_HEADERS})

set( LIBRARY_INCLUDE_DIRS_MPI_CONJUGATE_GRADIENT_INVERSION ${CMAKE_SOURCE_DIR}/libraries/inversion/MPIConjugateGradientInversion/include CACHE INTERNAL "" )
target_include_directories(mpi_conjugate_gradient_inversion_library PUBLIC ${LIBRARY_INCLUDE_DIRS_MPI_CONJUGATE_GRADIENT_INVERSION})

target_link_libraries(
    mpi_conjugate_gradient_inversion_library
    utilities_library
    core_library)
