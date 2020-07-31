#file(GLOB CPP_SOURCES *.cpp)
#file(GLOB H_HEADERS ../include/*.h)

#add_library(factory_library ${CPP_SOURCES} ${H_HEADERS})
add_library(forwardModelContainer_library
    forwardModelContainer.cpp ../include/forwardModelContainer.h)

set( LIBRARY_INCLUDE_DIRS_FACTORY ${CMAKE_SOURCE_DIR}/libraries/forwardModelContainer/include CACHE INTERNAL "" )
target_include_directories(forwardModelContainer_library PUBLIC ${LIBRARY_INCLUDE_DIRS_FACTORY} 
PRIVATE ${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include 
PRIVATE ${CMAKE_SOURCE_DIR}/libraries/inversion/interface/include 
PRIVATE ${CMAKE_SOURCE_DIR}/libraries/utilities/include)

target_link_libraries(
    forwardModelContainer_library
    
    core_library

    integral_forward_model_library
    finite_difference_forward_model_library
    
    conjugate_gradient_inversion_library
    gradient_descent_inversion_library
    random_inversion_library
    evolution_inversion_library
    OpenMP_gradient_descent_inversion_library)
