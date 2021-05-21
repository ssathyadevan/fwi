find_package(Doxygen)

if (Doxygen_FOUND)
	message("Doxygen found")
	#Set the file that contains the mainpage for the doxygen documentation
	set(MAINPAGE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Mainpage.dox)
	#Set the file that contains the documentation for the namespaces
	set(NAMESPACESINFO_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Namespaces.dox)
	#Set the path to the library
	set(LIBRARY_PATH ${CMAKE_SOURCE_DIR}/libraries)

	#Find all header files in the libraries folder and include the files for the mainpage and the namespaces info file
	file(GLOB_RECURSE INPUT_FILES_FOR_DOXYGEN ${LIBRARY_PATH}/*.h ${LIBRARY_PATH}/*.cpp ${MAINPAGE_FILE} ${NAMESPACESINFO_FILE})

	#Configuration parameters for doxygen
	set(DOXYGEN_INPUT_DIR "${LIBRARY_PATH} \\ \n ${MAINPAGE_FILE} \\ \n ${NAMESPACESINFO_FILE}")
	set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doxygen)
	set(DOXYGEN_INDEX_FILE ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/html/index.html)
	set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
	set(DOXYFILE_OUT ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
	set(PROJECT_NAME "Full Waveform Inversion")
	set(RECURSIVE YES)
	set(GENERATE_TREEVIEW YES)
	set(DISTRIBUTE_GROUP_DOC YES)
	set(EXTRACT_ALL YES)
	set(EXTRACT_PRIVATE YES)
	set(EXTRACT_STATIC YES)
	set(CLASS_GRAPH YES)
	set(COLLABORATION_GRAPH YES)
	set(UML_LOOK YES)
	set(UML_LIMIT_NUM_FIELDS 50)
	set(HAVE_DOT YES)
	set(INTERACTIVE_SVG YES)
	set(MAX_DOT_GRAPH_DEPTH 0)
	set(DOT_GRAPH_MAX_NODES 100)
	set(USE_PDFLATEX YES)
	set(PDF_HYPERLINKS YES)
	set(CALL_GRAPH YES)
	set(CALLER_GRAPH YES)

	#Define files and paths to be excluded from the doxygen documentation
	set(EXCLUDE_NLOHMANN_JSON_LIBRARY_FILE ${CMAKE_SOURCE_DIR}/libraries/io/include/json.h)
	set(EXCLUDE_OPENCL_LIBRARY_PATH */CL/*)
	set(EXCLUDE_TESTS_PATHS */test/*)
	set(EXCLUDE_FILE ${EXCLUDE_NLOHMANN_JSON_LIBRARY_FILE})
	set(EXCLUDE_PATTERN "${EXCLUDE_OPENCL_LIBRARY_PATH} \\ \n ${EXCLUDE_TESTS_PATHS}")

	#Replace variables inside @@ with the current values
	configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

	#Create output folder
	file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR}) #Doxygen won't create this for us

	add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
					   DEPENDS ${INPUT_FILES_FOR_DOXYGEN}
					   COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
					   MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
					   COMMENT "Generating docs"
					   VERBATIM)

	add_custom_target(doxygen DEPENDS ${DOXYGEN_INDEX_FILE})
endif(Doxygen_FOUND)
