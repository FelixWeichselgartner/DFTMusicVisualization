cmake_minimum_required(VERSION 3.14)
project(DFTMusicVisualization)
 
file(GLOB SOURCES 
    "*.h"
    "*.c"
    "lib/rpi_ws281x/*.c"
    "lib/rpi_ws281x/*.h"
    "mylib/DiscreteFourierTransformation/c/dft.c"
    "mylib/DiscreteFourierTransformation/c/dft.h"
    "mylib/mcp3008/mcp3008.c"
    "mylib/mcp3008/mcp3008.h"
)
 
add_executable(DFTMusicVisualization ${SOURCES})

target_link_libraries(DFTMusicVisualization m wiringPi)
