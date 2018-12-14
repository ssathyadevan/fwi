include_directories( ${LIBRARY_INCLUDE_DIRS_FORWARDMODEL} )
add_executable( FWI_PreProcess main.cpp )
target_link_libraries( FWI_PreProcess forwardModel_library )
install( TARGETS FWI_PreProcess DESTINATION bin)

