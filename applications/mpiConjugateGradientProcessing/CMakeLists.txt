add_executable( FWI_MPIProcess main.cpp )

target_link_libraries( FWI_MPIProcess
    factory_library 
    core_library
    conjugate_gradient_inversion_library)
    
install( TARGETS FWI_MPIProcess DESTINATION bin)
