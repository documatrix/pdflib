set (DATADIR "${CMAKE_INSTALL_PREFIX}/share")
set (PKGDATADIR "${DATADIR}/pdflib")
set (GETTEXT_PACKAGE "pdflib")
set (CMAKE_C_FLAGS "")
set (PREFIX ${CMAKE_INSTALL_PREFIX})
set (DOLLAR "$")

configure_file (${CMAKE_SOURCE_DIR}/src/Config.vala.cmake ${CMAKE_BINARY_DIR}/src/Config.vala)
add_definitions(-DGETTEXT_PACKAGE=\"${GETTEXT_PACKAGE}\")
add_definitions (${DEPS_CFLAGS})
include_directories( ${CMAKE_SOURCE_DIR}/src )
link_libraries ( ${DEPS_LIBRARIES} )
link_libraries ( ${DM_POPPLER_NAME} )
link_directories ( ${DEPS_LIBRARY_DIRS} )
find_package (Vala REQUIRED)
include( UseVala )

set (VALA_PACKAGES ${VALA_PACKAGES} poppler-glib)
set (VALA_PACKAGES ${VALA_PACKAGES} dmtestlib-1.0)

set (TEST_PDFLIB_SOURCES ${TEST_PDFLIB_SOURCES} test_pdflib.vala)

vala_precompile(TEST_PDFLIB_VALA_C
  ${TEST_PDFLIB_SOURCES}
PACKAGES
  ${VALA_PACKAGES} ${DM_POPPLER_NAME}
OPTIONS
  --thread -g --vapidir=${CMAKE_SOURCE_DIR}/src/ --vapidir=${CMAKE_INSTALL_PREFIX}/share/vala/vapi
  ${VAPIDIRS_ARGS}
CUSTOM_VAPIS
  ${CUSTOM_VAPIS_LIST}
)

add_executable(
  test_pdflib
  ${TEST_PDFLIB_VALA_C}
)

ADD_DEPENDENCIES( test_pdflib test_pdflib_preprocessed )
ADD_CUSTOM_TARGET( test_pdflib_preprocessed DEPENDS test_pdflib.vala )
#ADD_CUSTOM_COMMAND(
#    SOURCE tests/test_dm_poppler.vala
#    OUTPUT tests/test_dm_poppler.vala
#    COMMAND ${PREPROCESS_LOGGER} ${CMAKE_SOURCE_DIR}/tests/ ../log/messages.mdb dm_poppler
#)

install(TARGETS
  test_pdflib
RUNTIME DESTINATION
  bin/
)

if(HAVE_VALADOC)
  valadoc(pdflib
    ${CMAKE_BINARY_DIR}/valadoc/pdflib
    ${APP_SOURCES}
  PACKAGES
    ${VALA_PACKAGES}
  CUSTOM_VAPIS
    ${CUSTOM_VAPIS_LIST}
  )
  install(DIRECTORY
    ${CMAKE_BINARY_DIR}/valadoc
  DESTINATION
    share/doc/pdflib
  )
endif()
