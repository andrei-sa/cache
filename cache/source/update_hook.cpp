#include <update_hook.h>

namespace cache
{

  constexpr void UpdateHook::operator()(value_t value) const noexcept
  {
    return m_impl(value);
  }

}
