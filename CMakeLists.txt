cmake_minimum_required(VERSION 2.8.9)
project(DFTMusicVisualization)
 
file(GLOB SOURCES 
    "*.h"
    "*.c"
    "lib/rpi_ws281x/*.c"
    "lib/rpi_ws281x/*.h"
    "mylib/DiscreteFourierTransformation/*.c"
    "mylib/DiscreteFourierTransformation/*.h"
    "mylib/mcp3008/*.c"
    "mylib/mcp3008/*.h"    
)
 
add_executable(DFTMusicVisualization ${SOURCES})