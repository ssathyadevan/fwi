add_library(forward_model_base_library
	ForwardModelBase.cpp        ../include/ForwardModelBase.h)
	
target_include_directories(forward_model_base_library 
PRIVATE ${CMAKE_SOURCE_DIR}/libraries/forwardModel/base/include
PRIVATE ${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include 
PRIVATE ${CMAKE_SOURCE_DIR}/libraries/io/include)
target_link_libraries(forward_model_base_library PRIVATE core_library )
