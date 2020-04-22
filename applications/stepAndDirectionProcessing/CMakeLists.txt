add_executable( FWI_StepAndDirectionProcess main.cpp )

target_link_libraries( FWI_StepAndDirectionProcess
    factory_library 
    utilities_library
    step_and_direction_reconstructor_library)
    
install( TARGETS FWI_StepAndDirectionProcess DESTINATION bin)
