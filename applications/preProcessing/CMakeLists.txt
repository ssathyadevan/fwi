include_directories( ${LIBRARY_INCLUDE_DIRS_FORWARDMODEL} )
add_executable( FWI_PreProcess main_preProcessing.cpp )
target_link_libraries( FWI_PreProcess forwardModel_library )

