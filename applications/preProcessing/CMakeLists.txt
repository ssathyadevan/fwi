include_directories( ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARD_MODEL} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )

add_executable( FWI_PreProcess main.cpp )
target_link_libraries( FWI_PreProcess integral_forward_model_library core_library utilities_library)
install( TARGETS FWI_PreProcess DESTINATION bin)

