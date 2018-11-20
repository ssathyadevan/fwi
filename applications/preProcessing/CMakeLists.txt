include_directories( ${LIBRARY_INCLUDE_DIRS} )
add_executable( FWI_PreProcess main_preProcessing.cpp )
target_link_libraries( FWI_PreProcess inversion_library )

