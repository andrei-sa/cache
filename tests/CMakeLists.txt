set (SRC 
  cache_tests.cpp
  item_factory_tests.cpp
  item_file_tests.cpp
  lock_free_item_tests.cpp
  main.cpp
  reader_tests.cpp
  shared_lock_based_item_tests.cpp
  unique_lock_based_item_tests.cpp
  update_hook_tests.cpp
  writer_tests.cpp
)

file (GLOB DATA "data/*")
file (COPY ${DATA} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable (tests ${SRC})

target_link_libraries (tests cache)
target_link_libraries (tests file)

target_link_libraries (tests ${GTEST_LIBRARIES})

if (UNIX)
  target_link_libraries (tests atomic)
  target_link_libraries (tests pthread)
endif ()
