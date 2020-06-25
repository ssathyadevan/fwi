add_library(OpenMP_gradient_descent_inversion_library
    OpenMPgradientDescentInversion.cpp                    ../include/OpenMPgradientDescentInversion.h)

set( LIBRARY_INCLUDE_DIRS_OPENMP_GRADIENT_DESCENT_INVERSION ${CMAKE_SOURCE_DIR}/libraries/inversion/OpenMPgradientDescentInversion/include CACHE INTERNAL "" )
target_include_directories(OpenMP_gradient_descent_inversion_library PUBLIC ${LIBRARY_INCLUDE_DIRS_OPENMP_GRADIENT_DESCENT_INVERSION})

target_link_libraries(
    OpenMP_gradient_descent_inversion_library
    gradient_descent_inversion_library
    core_library
    integral_forward_model_library
    forwardModelContainer_library)
