find_package(Threads REQUIRED)
file(GLOB SOURCES *.c)
include_directories(../deps)
add_executable(needle_tests ${SOURCES})
target_link_libraries(needle_tests LINK_PUBLIC needle ${CMAKE_THREAD_LIBS_INIT})

