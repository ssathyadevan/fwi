
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARDMODEL} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FINITE_DIFFERENCE_FORWARDMODEL} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FACTORY} )

add_executable( FWI_UnifiedProcess main.cpp )
target_link_libraries( FWI_UnifiedProcess factory_library integral_forwardModel_library finite_difference_forwardModel_library core_library)
install( TARGETS FWI_UnifiedProcess DESTINATION bin)
