include_directories( ${LIBRARY_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FORWARDMODEL} )

add_executable( FWI_Process main.cpp )
target_link_libraries( FWI_Process inversion_library forwardModel_library)
install( TARGETS FWI_Process DESTINATION bin)
