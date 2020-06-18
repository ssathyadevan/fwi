include_directories(${CMAKE_CURRENT_SOURCE_DIR}/interface/include ${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include)##remove later
add_subdirectory(conjugateGradientInversion)
add_subdirectory(randomInversion)
add_subdirectory(gradientDescentInversion)
add_subdirectory(evolutionInversion)
#if(MPI_FOUND)
#    add_subdirectory(MPIConjugateGradientInversion)
#endif(MPI_FOUND)
if (OpenMP_CXX_FOUND)
    add_subdirectory(OpenMPgradientDescentInversion)
endif(OpenMP_CXX_FOUND)

