file(GLOB CPP_SOURCES *.cpp)
file(GLOB H_HEADERS include/*.h)

add_library(gradient_descent_inversion_library ${CPP_SOURCES} ${H_HEADERS})

set( LIBRARY_INCLUDE_DIRS_GRADIENT_DESCENT_INVERSION ${CMAKE_SOURCE_DIR}/libraries/inversion/gradientDescentInversion/include CACHE INTERNAL "" )
target_include_directories(gradient_descent_inversion_library PUBLIC ${LIBRARY_INCLUDE_DIRS_GRADIENT_DESCENT_INVERSION})

target_link_libraries(
    gradient_descent_inversion_library
    core_library
    integral_forward_model_library)
