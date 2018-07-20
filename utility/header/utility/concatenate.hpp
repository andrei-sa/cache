#pragma once

#include <iomanip>
#include <sstream>
#include <string>

namespace utility
{

  static std::string concatenate()
  {
    return "";
  }
  
  template<typename T>
  static std::string concatenate(const T& substr)
  {
    std::stringstream ss;
    ss << std::setprecision(17) << substr;
    return ss.str();
  }
  
  template<typename T, typename ...Args>
  static std::string concatenate(const T& substr, const Args&... rest)
  {
    std::stringstream ss;
    ss << std::setprecision(17) << substr;
    return ss.str() + concatenate(rest...);
  }
  
}
