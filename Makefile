
NUM_CPUS=${shell grep -c '^processor' /proc/cpuinfo | wc -l}
BUILD_DIR=build/
TEST_COMMAND=./${BUILD_DIR}/tests/needle_tests


.PHONY: clean build all install tests

clean:
	rm -rf ${BUILD_DIR}

build:
	mkdir -p ${BUILD_DIR}
	cd ${BUILD_DIR} && cmake ..
	make -C ${BUILD_DIR} -j ${NUM_CPUS}

all: build

install: build
	sudo make -C ${BUILD_DIR} install

tests: build
	${TEST_COMMAND}

