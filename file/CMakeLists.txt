set (SRC 
  source/item_file.cpp
  source/reader.cpp
  source/writer.cpp
)

include_directories (header/file)

add_library (file SHARED ${SRC})
