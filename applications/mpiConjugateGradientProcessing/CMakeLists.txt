add_executable( FWI_MPIProcess main.cpp )

target_link_libraries( FWI_MPIProcess
    integral_forward_model_library 
    finite_difference_forward_model_library
    factory_library
    core_library
    mpi_conjugate_gradient_inversion_library)
    
install( TARGETS FWI_MPIProcess DESTINATION bin)
