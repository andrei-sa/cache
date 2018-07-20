#pragma once

#include <string>

namespace utility
{

  template<typename T, typename ...Args>
  static std::string concatenate(const T& substr, const Args&... rest);
  
}

#include <utility/concatenate.hpp>
