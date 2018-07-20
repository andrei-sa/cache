#pragma once

#include <shared_mutex>

namespace utility
{

  // std::shared_mutex is unavailable prior to the C++17 standard
  using SharedMutex = std::shared_timed_mutex;

}
