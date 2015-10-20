set (DATADIR "${CMAKE_INSTALL_PREFIX}/share")
set (PKGDATADIR "${DATADIR}/pdflib")
set (GETTEXT_PACKAGE "pdflib")
set (CMAKE_C_FLAGS "-g -O3")
set (CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})
set (PREFIX ${CMAKE_INSTALL_PREFIX})
set (DOLLAR "$")

configure_file (${CMAKE_SOURCE_DIR}/src/Config.vala.cmake ${CMAKE_BINARY_DIR}/src/Config.vala)
add_definitions(-DGETTEXT_PACKAGE=\"${GETTEXT_PACKAGE}\")
find_package (Vala REQUIRED)
include( UseVala )


if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
  include_directories( ${CMAKE_INSTALL_PREFIX}/include/poppler )
else()
  include_directories( /usr/include/poppler )
endif()

set (VALA_PACKAGES ${VALA_PACKAGES} poppler-glib)
set (VALA_PACKAGES ${VALA_PACKAGES} gio-2.0)
set (VALA_PACKAGES ${VALA_PACKAGES} glib-2.0)
set (VALA_PACKAGES ${VALA_PACKAGES} gobject-2.0)
set (VALA_PACKAGES ${VALA_PACKAGES} dmtestlib-1.0)
set (VALA_PACKAGES ${VALA_PACKAGES} posix)
set (VALA_PACKAGES ${VALA_PACKAGES} open_dmlib-1.0)
set (VALA_PACKAGES ${VALA_PACKAGES} open_dmlib_io)
set (VALA_PACKAGES ${VALA_PACKAGES} zlib)

set (PDFLIB_SOURCES pdflib.vala)

set (DM_POPPLER_SOURCES ${DM_POPPLER_SOURCES} ForOutputDev.cc)
set (DM_POPPLER_SOURCES ${DM_POPPLER_SOURCES} dm_poppler.cc)

if(${LOG_DEBUG} STREQUAL true)
  set (CMAKE_VALA_OPTS ${CMAKE_VALA_OPTS} -D LOG_DEBUG)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D LOG_DEBUG")
endif()

add_library(
  ${DM_POPPLER_NAME} SHARED
  ${DM_POPPLER_SOURCES}
)

link_libraries ( ${DM_POPPLER_NAME} )

vala_precompile(PDFLIB_VALA_C
  ${PDFLIB_SOURCES}
PACKAGES
  ${VALA_PACKAGES}
  ${DM_POPPLER_NAME}
GENERATE_VAPI
  ${PDFLIB_NAME}
CUSTOM_VAPIS
  ${CUSTOM_VAPIS_LIST}
GENERATE_HEADER
  pdflib
OPTIONS
  --thread -g  --vapidir=${CMAKE_SOURCE_DIR}/src --vapidir=${CMAKE_INSTALL_PREFIX}/share/vala/vapi
  ${CMAKE_VALA_OPTS} ${VAPIDIRS_ARGS}
)

add_library(
  ${PDFLIB_NAME} SHARED
  ${PDFLIB_VALA_C}
)

# CREATE PKGCONFIG FILES
if(NOT DEFINED CMAKE_INSTALL_BINDIR)
  set(CMAKE_INSTALL_BINDIR bin)
endif(NOT DEFINED CMAKE_INSTALL_BINDIR)
if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR lib)
endif(NOT DEFINED CMAKE_INSTALL_LIBDIR)
set(prefix       ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${CMAKE_INSTALL_PREFIX})
if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
  set(libdir       ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR})
else()
  set(libdir       ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
endif()
set(includedir  ${CMAKE_INSTALL_PREFIX}/include)
set(LIBS         "")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libpdflib.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PDFLIB_NAME}.pc @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libdm_poppler.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/${DM_POPPLER_NAME}.pc @ONLY)

  # Install pdflib
if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
  install(
    TARGETS
      ${PDFLIB_NAME}
      ${DM_POPPLER_NAME}
    DESTINATION
    bin/
  )
else()
  install(
    TARGETS
      ${PDFLIB_NAME}
      ${DM_POPPLER_NAME}
    LIBRARY DESTINATION
      lib/
  )
endif( )

# Install pdflib header files
install(
FILES
  ${CMAKE_CURRENT_BINARY_DIR}/pdflib.h
DESTINATION
  include/${PDFLIB_NAME}/
)

# Install dm_poppler header files
install(
FILES
  ${CMAKE_CURRENT_BINARY_DIR}/ForOutputDev.h
  ${CMAKE_CURRENT_BINARY_DIR}/dm_poppler.h
DESTINATION
  include/${DM_POPPLER_NAME}/
)

# Install vapi files
install(
FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${DM_POPPLER_NAME}.vapi
  ${CMAKE_CURRENT_BINARY_DIR}/${PDFLIB_NAME}.vapi
DESTINATION
  ${CMAKE_INSTALL_PREFIX}/share/vala/vapi/
)

# Install pkg-config file...
install( FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${DM_POPPLER_NAME}.pc
  ${CMAKE_CURRENT_BINARY_DIR}/${PDFLIB_NAME}.pc
DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/pkgconfig
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
