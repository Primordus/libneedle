
SCONS=scons
SCONS_FLAGS=--jobs=${shell grep -c '^processor' /proc/cpuinfo} -Q
LIB_DIR=bin/src/
TEST_ENV=LD_LIBRARY_PATH=${LIB_DIR} 
TEST_COMMAND=./bin/tests/mt_tests


build:
	${SCONS} ${SCONS_FLAGS}

tests: build
	${TEST_ENV} ${TEST_COMMAND}

clean:
	${SCONS} ${SCONS_FLAGS} --clean

.PHONY: clean

