file(GLOB SOURCES *.cpp)
add_library(needle SHARED ${SOURCES})
install(TARGETS needle DESTINATION /usr/lib)

