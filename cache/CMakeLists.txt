set (SRC 
  source/lock_policy.cpp
  source/lock_policy/read_heavy_lock_policy.cpp
  source/lock_policy/write_heavy_lock_policy.cpp
)

include_directories (header/cache)

add_library (cache SHARED ${SRC})
