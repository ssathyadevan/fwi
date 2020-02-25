add_library(OpenMPI_gradient_descent_inversion_library
    OpenMPIgradientDescentInversion.cpp                    ../include/OpenMPIgradientDescentInversion.h)

set( LIBRARY_INCLUDE_DIRS_OPENMPI_GRADIENT_DESCENT_INVERSION ${CMAKE_SOURCE_DIR}/libraries/inversion/OpenMPIgradientDescentInversion/include CACHE INTERNAL "" )
target_include_directories(OpenMPI_gradient_descent_inversion_library PUBLIC ${LIBRARY_INCLUDE_DIRS_OPENMPI_GRADIENT_DESCENT_INVERSION})

target_link_libraries(
    OpenMPI_gradient_descent_inversion_library
    gradient_descent_inversion_library
    utilities_library
    core_library
    integral_forward_model_library)
