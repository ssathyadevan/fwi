add_executable( FWI_OpenMPGradienDescentProcess main.cpp )

target_link_libraries( FWI_OpenMPGradienDescentProcess
    integral_forward_model_library
    finite_difference_forward_model_library
    OpenMP_gradient_descent_inversion_library
    utilities_library)
    
install( TARGETS FWI_OpenMPGradienDescentProcess DESTINATION bin)
