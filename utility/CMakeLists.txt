set (SRC 
  source/exceptions.cpp
)

include_directories (header/utility)

add_library (utility SHARED ${SRC})

