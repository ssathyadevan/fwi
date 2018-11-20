include_directories( ${LIBRARY_INCLUDE_DIRS} )
add_executable( FWI_Process main_processing.cpp )
target_link_libraries( FWI_Process inversion_library )
