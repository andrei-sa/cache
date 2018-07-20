#include <exceptions.h>

#include <iostream>

namespace utility
{

  void print_exception(const std::exception& e)
  {
    std::cerr << e.what() << std::endl;

    try
    {
      std::rethrow_if_nested(e);
    }
    catch (const std::exception& e)
    {
      print_exception(e);
    }
    catch (...)
    {
    }
  }

}
