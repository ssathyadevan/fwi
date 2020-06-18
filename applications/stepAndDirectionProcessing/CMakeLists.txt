add_executable( FWI_StepAndDirectionProcess main.cpp )

target_include_directories(FWI_StepAndDirectionProcess PRIVATE ${CMAKE_SOURCE_DIR}/libraries/inversion/interface/include)
target_link_libraries( FWI_StepAndDirectionProcess
    factory_library 
    utilities_library
    step_and_direction_reconstructor_library)
    
install( TARGETS FWI_StepAndDirectionProcess DESTINATION bin)
