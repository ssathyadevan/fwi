#Generate CMakeLists.txt that makes cmake download the gtest framework from github
configure_file(GoogleTestDownloadConfiguration.in ${CMAKE_BINARY_DIR}/googletest-download/CMakeLists.txt)
#Configure gtest framework download job using generated CMakeLists.txt
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
				RESULT_VARIABLE result
				WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
#Download gtest framework source files at configuration time
execute_process(COMMAND ${CMAKE_COMMAND} --build .
				RESULT_VARIABLE result
				WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )


