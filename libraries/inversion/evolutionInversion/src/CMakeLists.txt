file(GLOB CPP_SOURCES *.cpp)
file(GLOB H_HEADERS include/*.h)

add_library(evolution_inversion_library ${CPP_SOURCES} ${H_HEADERS})

set( LIBRARY_INCLUDE_DIRS_EVOLUTION_INVERSION ${CMAKE_SOURCE_DIR}/libraries/inversion/evolutionInversion/include CACHE INTERNAL "" )
target_include_directories(evolution_inversion_library PUBLIC ${LIBRARY_INCLUDE_DIRS_EVOLUTION_INVERSION})

target_link_libraries(
    evolution_inversion_library
    core_library
    integral_forward_model_library)
