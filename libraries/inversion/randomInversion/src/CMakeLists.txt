add_library(random_inversion_library
    randomInversion.cpp                     ../include/randomInversion.h
                                            ../include/randomInversionInput.h
    randomInversionInputCardReader.cpp      ../include/randomInversionInputCardReader.h)

set( LIBRARY_INCLUDE_DIRS_RANDOM_INVERSION ${CMAKE_SOURCE_DIR}/libraries/inversion/randomInversion/include CACHE INTERNAL "" )
target_include_directories(random_inversion_library PUBLIC ${LIBRARY_INCLUDE_DIRS_RANDOM_INVERSION})

target_link_libraries(
    random_inversion_library
    utilities_library
    core_library
    integral_forward_model_library)
