file(GLOB CPP_SOURCES ../src/*.cpp)
file(GLOB H_HEADERS *.h)

message( STATUS "TESTR: ${CPP_SOURCES}")

add_library( inversion_library ${CPP_SOURCES} ${H_HEADERS} )

