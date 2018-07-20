set (SRC 
  main.cpp
)

add_executable (main ${SRC})

target_link_libraries (main cache)
target_link_libraries (main file)
target_link_libraries (main utility)

if (UNIX)
  target_link_libraries (main atomic)
  target_link_libraries (main pthread)
endif ()

