include_directories( ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARD_MODEL} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories(${CMAKE_SOURCE_DIR}/libraries/utilities/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/performance/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/forwardModel/base/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/io/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/factory/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/forwardModel/base/include)

find_package(MPI)

if(MPI_FOUND)
    add_executable( FWI_ProcessMPI main.cpp )
    target_compile_definitions( FWI_ProcessMPI PRIVATE -DMPI)
    add_definitions(-DMPI)
    target_include_directories(FWI_ProcessMPI PRIVATE ${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include PRIVATE ${CMAKE_SOURCE_DIR}/libraries/inversion/interface/include PRIVATE ${CMAKE_SOURCE_DIR}/libraries/utilities/include PRIVATE ${CMAKE_SOURCE_DIR}/libraries/performance/include PRIVATE ${CMAKE_SOURCE_DIR}/libraries/io/include)
    target_link_libraries( FWI_ProcessMPI factory_library step_and_direction_reconstructor_library integral_forward_model_library core_library integral_forward_model_library io_library performance_library)
    include_directories(SYSTEM ${MPI_INCLUDE_PATH})
    target_link_libraries(FWI_ProcessMPI ${MPI_CXX_LIBRARIES})
    install( TARGETS FWI_ProcessMPI DESTINATION bin)

endif()


