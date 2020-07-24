include_directories(${CMAKE_SOURCE_DIR}/deps)
file(GLOB CCD_SOURCES *.cpp)
add_library(CCD SHARED ${CCD_SOURCES})
# target_link_libraries(CCD /usr/bin/gcc -fPIC -shared  -Wl,-soname,libCCD.so -o libCCD.so )