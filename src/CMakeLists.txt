set (DATADIR "${CMAKE_INSTALL_PREFIX}/share")
set (PKGDATADIR "${DATADIR}/dm_logger")
set (GETTEXT_PACKAGE "dm_logger")
set (CMAKE_C_FLAGS "-g -O3")
set (PREFIX ${CMAKE_INSTALL_PREFIX})
set (DOLLAR "$")

configure_file (${CMAKE_SOURCE_DIR}/src/Config.vala.cmake ${CMAKE_BINARY_DIR}/src/Config.vala)
add_definitions(-DGETTEXT_PACKAGE=\"${GETTEXT_PACKAGE}\")
find_package (Vala REQUIRED)
include( UseVala )

set (VALA_PACKAGES ${VALA_PACKAGES} gio-2.0)
set (VALA_PACKAGES ${VALA_PACKAGES} glib-2.0)
set (VALA_PACKAGES ${VALA_PACKAGES} gobject-2.0)
set (VALA_PACKAGES ${VALA_PACKAGES} dmtestlib-1.0)
set (VALA_PACKAGES ${VALA_PACKAGES} posix)
set (VALA_PACKAGES ${VALA_PACKAGES} open_dmlib-1.0)
set (VALA_PACKAGES ${VALA_PACKAGES} open_dmlib_io)
set (VALA_PACKAGES ${VALA_PACKAGES} dm_logger-1.0)
set (VALA_PACKAGES ${VALA_PACKAGES} zlib)

set (PDF2FOR_SOURCES pdf2for.vala)

vala_precompile(PDF2FOR_VALA_C
  ${PDF2FOR_SOURCES}
PACKAGES
  ${VALA_PACKAGES}
CUSTOM_VAPIS
  ${CUSTOM_VAPIS_LIST}
OPTIONS
  --thread -g --vapidir=${CMAKE_INSTALL_PREFIX}/share/vala/vapi
  ${CMAKE_VALA_OPTS} ${VAPIDIRS_ARGS}
)

add_executable(
  pdf2for
  ${PDF2FOR_VALA_C}
)


ADD_DEPENDENCIES( pdf2for pdf2for_preprocessed )
ADD_CUSTOM_TARGET( pdf2for_preprocessed DEPENDS pdf2for.vala )
ADD_CUSTOM_COMMAND(
  SOURCE pdf2for.vala
  OUTPUT pdf2for.vala
  COMMAND perl ${PREPROCESS_LOGGER} ${CMAKE_SOURCE_DIR}/src/ ../log/messages.mdb pdf2for
)

# Install dm_logreader and preprocess_logger.pl
install(
TARGETS
  pdf2for
RUNTIME DESTINATION
  bin
)

# Install message database...
install(
FILES
  ${CMAKE_CURRENT_BINARY_DIR}/../log/messages.mdb
DESTINATION
  share/${PDF2FOR_NAME}/
)

 
if(HAVE_VALADOC)
  valadoc(dm_logger
    ${CMAKE_BINARY_DIR}/valadoc/pdf2for
    ${APP_SOURCES}
  PACKAGES
    ${VALA_PACKAGES}
  CUSTOM_VAPIS
    ${CUSTOM_VAPIS_LIST}
  )
  install(DIRECTORY
    ${CMAKE_BINARY_DIR}/valadoc
  DESTINATION
    share/doc/pdf2for
  )
endif()
