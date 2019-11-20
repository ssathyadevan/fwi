#file(GLOB CPP_SOURCES *.cpp)
#file(GLOB H_HEADERS ../include/*.h)

#add_library(factory_library ${CPP_SOURCES} ${H_HEADERS})
add_library(factory_library
    factory.cpp ../include/factory.h)

set( LIBRARY_INCLUDE_DIRS_FACTORY ${CMAKE_SOURCE_DIR}/libraries/factory/include CACHE INTERNAL "" )
target_include_directories(factory_library PUBLIC ${LIBRARY_INCLUDE_DIRS_FACTORY})

target_link_libraries(
    factory_library
    
    core_library

    integral_forward_model_library
    finite_difference_forward_model_library
    
    conjugate_gradient_inversion_library
    gradient_descent_inversion_library
    random_inversion_library
    evolution_inversion_library)