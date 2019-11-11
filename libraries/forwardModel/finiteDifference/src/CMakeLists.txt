file(GLOB CPP_SOURCES *.cpp)
file(GLOB H_HEADERS ../include/*.h)

add_library(finite_difference_forward_model_library ${CPP_SOURCES} ${H_HEADERS})

set( LIBRARY_INCLUDE_DIRS_FINITE_DIFFERENCE_FORWARD_MODEL ${CMAKE_SOURCE_DIR}/libraries/forwardModel/finiteDifference/include CACHE INTERNAL "" )
target_include_directories(finite_difference_forward_model_library PUBLIC ${LIBRARY_INCLUDE_DIRS_FINITE_DIFFERENCE_FORWARD_MODEL})

target_link_libraries(finite_difference_forward_model_library core_library)