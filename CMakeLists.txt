cmake_minimum_required(VERSION 3.31)
project(xLR C)

set(CMAKE_C_STANDARD 23)

#add_compile_options("-O3" "-Os")
add_compile_options("-funsigned-char")
add_compile_options("-Wall" "-Wextra" "-Werror")
if (WIN32)
    add_compile_options("-Wno-format")
endif ()
if (ENABLE_TEST_COV)
    add_compile_options("-fprofile-arcs" "-ftest-coverage")
endif ()
if (ENABLE_GPROF)
    add_compile_options("-pg")
    add_link_options("-pg")
endif ()

include_directories(meman)
aux_source_directory(meman MEMAN_SRC)
add_library(meman STATIC ${MEMAN_SRC})
link_libraries(meman)

add_library(xLR STATIC
        LRContext.c LRContext.h)
