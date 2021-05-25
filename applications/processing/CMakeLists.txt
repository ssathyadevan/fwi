include_directories( ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARD_MODEL} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories(${CMAKE_SOURCE_DIR}/libraries/utilities/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/performance/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/forwardModel/base/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/io/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/factory/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/forwardModel/base/include)

add_executable( FWI_Process main.cpp )
target_include_directories(FWI_Process PRIVATE ${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include PRIVATE ${CMAKE_SOURCE_DIR}/libraries/inversion/interface/include PRIVATE ${CMAKE_SOURCE_DIR}/libraries/utilities/include PRIVATE ${CMAKE_SOURCE_DIR}/libraries/performance/include PRIVATE ${CMAKE_SOURCE_DIR}/libraries/io/include)
target_link_libraries( FWI_Process factory_library integral_forward_model_library core_library integral_forward_model_library io_library performance_library)
install( TARGETS FWI_Process DESTINATION bin)

find_package(MPI) #make it REQUIRED, if you want
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
target_link_libraries(FWI_Process ${MPI_CXX_LIBRARIES})
