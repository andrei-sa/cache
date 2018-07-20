#pragma once

namespace cache
{

  template <typename KeyType, typename ValueType>
  template <
    typename FuncFwd,
    typename std::enable_if_t<
      !std::is_base_of<
        UpdateHook<
          KeyType, 
          ValueType
        >, 
        std::decay_t<FuncFwd>
      >::value
    >*
  >
  UpdateHook<KeyType, ValueType>::UpdateHook(FuncFwd&& func)
    : m_impl(std::forward<FuncFwd>(func))
  {
    static_assert(noexcept(func(KeyType {}, ValueType {})), "Update hook must be a noexcept function!");
  }

  template <typename KeyType, typename ValueType>
  void UpdateHook<KeyType, ValueType>::operator()(const KeyType& key, const ValueType& value) const noexcept
  {
    m_impl(key, value);
  }

  template <typename KeyType, typename ValueType, typename FuncFwd>
  UpdateHook<KeyType, ValueType> make_update_hook(FuncFwd&& func)
  {
    return UpdateHook<KeyType, ValueType>(std::forward<FuncFwd>(func));
  }

}
