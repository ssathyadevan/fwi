
add_library(conjugate_gradient_inversion_library
    conjugateGradientInversion.cpp                      ../include/conjugateGradientInversion.h
                                                        ../include/conjugateGradientInversionInput.h
    conjugateGradientInversionInputCardReader.cpp       ../include/conjugateGradientInversionInputCardReader.h
                                                        ../include/deltaAmplification.h
                                                        ../include/iter1.h
                                                        ../include/regularization.h)

set( LIBRARY_INCLUDE_DIRS_CONJUGATE_GRADIENT_INVERSION ${CMAKE_SOURCE_DIR}/libraries/inversion/conjugateGradientInversion/include CACHE INTERNAL "" )
target_include_directories(conjugate_gradient_inversion_library PUBLIC ${LIBRARY_INCLUDE_DIRS_CONJUGATE_GRADIENT_INVERSION})

target_link_libraries(
    conjugate_gradient_inversion_library
    utilities_library
    core_library
    integral_forward_model_library)
