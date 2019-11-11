file(GLOB CPP_SOURCES *.cpp)
file(GLOB H_HEADERS ../include/*.h)

add_library(integral_forward_model_library ${CPP_SOURCES} ${H_HEADERS})

set( LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARD_MODEL ${CMAKE_SOURCE_DIR}/libraries/forwardModel/integral/include CACHE INTERNAL "" )
target_include_directories(integral_forward_model_library PUBLIC ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARD_MODEL})

target_link_libraries(integral_forward_model_library core_library)