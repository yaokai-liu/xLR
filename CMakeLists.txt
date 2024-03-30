cmake_minimum_required(VERSION 3.28)
project(xLR C)

set(CMAKE_C_STANDARD 23)

add_compile_options("-O3")

set(CMAKE_INSTALL_POSTFIX "D:/usr")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/lib")

include_directories(.)

add_library(xLR STATIC
        Parser.h Parser.c
        ActionTable.h ActionTable.c
        TrieTree.h TrieTree.c
        BinTree.c BinTree.h
        Set.c Set.h
)
link_libraries(xLR)

add_executable(test-lex test/test-lex.c)
add_executable(test-table test/test-table.c)
add_executable(test-parse test/test-parse.c)

