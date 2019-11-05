file(GLOB CPP_SOURCES src/*.cpp)
file(GLOB H_HEADERS include/*.h)

set( LIBRARY_INCLUDE_DIRS_FACTORY "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" )
include_directories( ${LIBRARY_INCLUDE_DIRS_FACTORY} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CONJUGATE_GRADIENT_INVERSION} )
include_directories( ${LIBRARY_INCLUDE_DIRS_RANDOM_INVERSION} )
include_directories( ${LIBRARY_INCLUDE_DIRS_EVOLUTION_INVERSION} )
include_directories( ${LIBRARY_INCLUDE_DIRS_GRADIENT_DESCENT_INVERSION} )
include_directories( ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARDMODEL} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FINITE_DIFFERENCE_FORWARDMODEL} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )

add_library( factory_library ${CPP_SOURCES} ${H_HEADERS} )
target_link_libraries (factory_library Eigen3::Eigen)


target_link_libraries( factory_library
			conjugate_gradient_inversion_library
			random_inversion_library
			evolution_inversion_library
			gradient_descent_inversion_library)


