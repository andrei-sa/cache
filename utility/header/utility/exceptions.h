#pragma once

#include <utility/concatenate.h>

#include <stdexcept>

namespace utility
{

  void print_exception(const std::exception& e);

}

#define THROW(...) throw (\
  std::runtime_error(\
    utility::concatenate(\
      __VA_ARGS__\
    )\
  )\
)

#define THROW_IF(cond, ...) if (cond) THROW(__VA_ARGS__)

#define RETHROW(...) std::throw_with_nested(utility::concatenate(__VA_ARGS__))
