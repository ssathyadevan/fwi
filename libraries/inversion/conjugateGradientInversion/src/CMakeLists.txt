file(GLOB CPP_SOURCES *.cpp)
file(GLOB H_HEADERS ../include/*.h)

add_library(conjugate_gradient_inversion_library ${CPP_SOURCES} ${H_HEADERS})

set( LIBRARY_INCLUDE_DIRS_CONJUGATE_GRADIENT_INVERSION ${CMAKE_SOURCE_DIR}/libraries/inversion/conjugateGradientInversion/include CACHE INTERNAL "" )
target_include_directories(conjugate_gradient_inversion_library PUBLIC ${LIBRARY_INCLUDE_DIRS_CONJUGATE_GRADIENT_INVERSION})

target_link_libraries(
    conjugate_gradient_inversion_library
    core_library
    integral_forward_model_library)