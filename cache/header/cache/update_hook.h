#pragma once

#include <functional>

namespace cache
{

  /**
   * \class UpdateHook
   * \brief Adaptor for a noexcept function object with two arguments
   * \details Used for file updates upon destruction of cache items
   * \tparam KeyType - type of keys in Cache
   * \tparam ValueType - type of values in Cache
   */
  template <typename KeyType, typename ValueType>
  class UpdateHook
  {
  private:
    using Impl = std::function<void(const KeyType&, const ValueType&)>;

  public:
    /**
     * \brief Conversion constructor
     * \param func - noexcept function object with void return type taking KeyType and ValueType as arguments
     * \details Since noexcept is not part of function type before C++17, raw function pointers and its
     * users (e.g. std::function) cannot be used as func. Only noexcept explicit noexcept functors and
     * closures can be used in C++14 and before. Move constructor overloading is disabled.
     */ 
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
      >* = nullptr
    >
    UpdateHook(FuncFwd&& func);

    /**
     * \brief Passes the arguments to the function object forwarded in constructor
     */
    void operator()(const KeyType& key, const ValueType& value) const noexcept;

  private:
    Impl m_impl;
  };

  template <typename KeyType, typename ValueType, typename FuncFwd>
  UpdateHook<KeyType, ValueType> make_update_hook(FuncFwd&& func);

}

#include <cache/update_hook.hpp>
