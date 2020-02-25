add_executable( FWI_OpenMPIGradienDescentProcess main.cpp )

target_link_libraries( FWI_OpenMPIGradienDescentProcess 
    integral_forward_model_library
    finite_difference_forward_model_library
    OpenMPI_gradient_descent_inversion_library
    utilities_library)
    
install( TARGETS FWI_OpenMPIGradienDescentProcess DESTINATION bin)
