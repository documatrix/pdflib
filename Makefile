CP=cp -u -r -p
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  CP=rsync -au
endif

test_quiet:TRVPARAM=-f
test_detail:TRVPARAM=-d
test_qd:TRVPARAM=-f -d

CMAKE_PREFIX=
ifeq "${MAKECMDGOALS}" "windows"
  CMAKE_PREFIX=-DCMAKE_INSTALL_PREFIX=/usr/i686-w64-mingw32/sys-root/mingw
endif

ifeq "${MAKECMDGOALS}" "windows64"
  CMAKE_PREFIX=-DCMAKE_INSTALL_PREFIX=/usr/x86_64-w64-mingw32/sys-root/mingw
endif

ifneq "${PREFIX}" ""
  CMAKE_PREFIX=-DCMAKE_INSTALL_PREFIX=${PREFIX}
endif

ifeq "${TARGET_GLIB}" ""
  TARGET_GLIB=2.32
endif
CMAKE_VALA_OPTS=--target-glib=${TARGET_GLIB}

DEBUG ?= 0
ifeq ($(DEBUG), 1)
  LOG_DEBUG=true
else
  LOG_DEBUG=false
endif

CMAKE_OPTS=${CMAKE_PREFIX} -DCMAKE_VALA_OPTS=${CMAKE_VALA_OPTS} -DVAPIDIRS=${VAPIDIRS} -DTARGET_GLIB=${TARGET_GLIB} -DLOG_DEBUG=${LOG_DEBUG} -DCMAKE_INSTALL_RPATH=\$$ORIGIN/../lib

all: linux

copy_files:
	${CP} cmake build/
	${CP} doc build/
	${CP} src build/
	${CP} include/* build/src/
	${CP} tests build/
	${CP} CMakeLists.txt build/
	find build/ -name CMakeCache.txt -delete

linux: build copy_files
	cd build && cmake . ${CMAKE_OPTS} && make

windows: build copy_files
	cd build && cmake . -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-mingw32.cmake ${CMAKE_OPTS} && make

windows64: build copy_files
	cd build && cmake . -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-mingw64.cmake ${CMAKE_OPTS} && make

install: build
	cd build && make install && ldconfig

clean: build
	rm -rf build

build:
	mkdir build
	mkdir build/log

testdir: build
	mkdir -p build/testdir

test: testdir
	cd build/tests && gtester ./test_pdflib -k -o ../testdir/ergebnis.xml || exit 0
	cd build && trv ${TRVPARAM} -i testdir/ergebnis.xml

test_quiet: test
test_detail: test
test_qd: test
