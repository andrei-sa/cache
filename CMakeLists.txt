cmake_minimum_required (VERSION 3.0.0 FATAL_ERROR)

macro (add_target TARGET_NAME)
  include_directories (${TARGET_NAME}/header)
  set (SUBDIRECTORY_LIST "${TARGET_NAME};${SUBDIRECTORY_LIST}")
endmacro ()

macro (add_subdirectories)
  foreach (SUBDIRECTORY ${SUBDIRECTORY_LIST})
    add_subdirectory (${SUBDIRECTORY})
  endforeach ()
endmacro ()

project (Cache)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/runtime)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/runtime)
message ("Output directory: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

include_directories (.)

find_package (GTest)

add_target (cache)
add_target (file)
add_target (main)
add_target (utility)

if (${GTest_FOUND})
  add_target (tests)
else ()
  message ("Tests will not be built as GTest is not found!")
endif ()

add_subdirectories ()
