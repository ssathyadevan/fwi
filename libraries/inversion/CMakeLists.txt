
add_subdirectory(conjugateGradientInversion)
add_subdirectory(randomInversion)
add_subdirectory(gradientDescentInversion)
add_subdirectory(evolutionInversion)
if(MPI_FOUND)
    add_subdirectory(MPIConjugateGradientInversion)
endif(MPI_FOUND)
add_subdirectory(OpenMPgradientDescentInversion)


