
NUM_CPUS=${shell grep -c '^processor' /proc/cpuinfo}
BUILD_DIR=build/
TEST_COMMAND=./${BUILD_DIR}/tests/needle_tests


.PHONY: all clean build install tests

all: build

clean:
	rm -rf ${BUILD_DIR}

build:
	mkdir -p ${BUILD_DIR}
	cd ${BUILD_DIR} && cmake ..
	make -C ${BUILD_DIR} -j ${NUM_CPUS} VERBOSE=1

install: build
	sudo make -C ${BUILD_DIR} install

tests: build
	${TEST_COMMAND}

