add_library(forward_model_interface_library
	forwardModelInterface.cpp        ../include/forwardModelInterface.h)
	
target_include_directories(forward_model_interface_library PRIVATE ${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include)
target_link_libraries(forward_model_interface_library PRIVATE core_library )