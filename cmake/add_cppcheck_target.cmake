function( add_cppcheck_target )
	find_program(PATH_TO_CPPCHECK_EXECUTABLE NAMES cppcheck)
	message(STATUS "Found Cppcheck: " ${PATH_TO_CPPCHECK_EXECUTABLE})

	set(OPTIONS)
	set(ONE_VALUE_ARGS OUTPUTFILE)
	set(MULTI_VALUE_ARGS INPUTFILES PARAMETERS)
	cmake_parse_arguments(PREF "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN})

	list(FILTER PREF_PARAMETERS EXCLUDE REGEX --output-file=.*)

	add_custom_target(	cppcheck 
						COMMAND ${PATH_TO_CPPCHECK_EXECUTABLE} ${PREF_INPUTFILES} ${PREF_PARAMETERS} --output-file=${PREF_OUTPUTFILE}
						MAIN_DEPENDENCY ${PREF_INPUTFILES})

	add_custom_command(	TARGET cppcheck 
						POST_BUILD
						COMMAND ${CMAKE_COMMAND} -E echo "Cppcheck output in: ${PREF_OUTPUTFILE}")
	message(STATUS "Cppcheck target added")
endfunction( add_cppcheck_target )