#set(CPP_SOURCES_AND_H_HEADERS 
#                                  	../include/utilityFunctions.h)

#add_library(utilities_library ${CPP_SOURCES_AND_H_HEADERS})

#set( LIBRARY_INCLUDE_DIRS_UTILITIES ${CMAKE_SOURCE_DIR}/libraries/utilities/include CACHE INTERNAL "" )
#target_include_directories(utilities_library PUBLIC ${LIBRARY_INCLUDE_DIRS_UTILITIES} PRIVATE ${CMAKE_SOURCE_DIR}/libraries/io/include)

#target_link_libraries(utilities_library PRIVATE Eigen3::Eigen)
