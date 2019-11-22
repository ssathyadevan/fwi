# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FACTORY} )
# Might need core and/or forward & inverison libraries too.
include_directories(../include)

# Build the tests!