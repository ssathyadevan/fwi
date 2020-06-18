add_executable( FWI_UnifiedProcess main.cpp )

target_include_directories(FWI_UnifiedProcess PRIVATE ${CMAKE_SOURCE_DIR}/libraries/inversion/interface/include)
target_link_libraries( FWI_UnifiedProcess
    factory_library 
    utilities_library)
    
install( TARGETS FWI_UnifiedProcess DESTINATION bin)
