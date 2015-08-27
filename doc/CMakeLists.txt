file(GLOB list_data RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)
foreach(file_data ${list_data})
	IF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${file_data})
		install(DIRECTORY
			${file_data}
		DESTINATION
			share/doc/dm_logger
		)
	ELSE()
		install(FILES
			${file_data}
		DESTINATION
			share/doc/dm_logger
		)
	ENDIF()
endforeach()

