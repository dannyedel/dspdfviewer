# Copy all the filex in QRCFILES into the binary dir,
# and execute qrc (Qt Resource Compiler) on them.

foreach(qrc IN LISTS QRCFILES)

	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${qrc}
		DEPENDS
			${CMAKE_CURRENT_SOURCE_DIR}/${qrc}
			${TRANSLATIONFILES}
		COMMAND cmake -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${qrc} ${CMAKE_CURRENT_BINARY_DIR}/${qrc}
	)

	set(TMP_OUTFILENAME "${CMAKE_CURRENT_BINARY_DIR}/${qrc}.cxx")

	set(RCCCOMPILER ${Qt5Core_RCC_EXECUTABLE})

	add_custom_command(
		OUTPUT ${TMP_OUTFILENAME}
		DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${qrc}
		COMMAND ${RCCCOMPILER} -o ${TMP_OUTFILENAME} ${CMAKE_CURRENT_BINARY_DIR}/${qrc}
	)

	unset(RCCCOMPILER)

	list(APPEND EMBEDDED_QRC
		${TMP_OUTFILENAME}
	)

	unset(TMP_OUTFILENAME)
endforeach()
