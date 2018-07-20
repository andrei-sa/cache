#pragma once

#include <cache/item.h>

#include <type_traits>

namespace cache
{

  /**
   * \brief Creates an item
   * \details This implementation is only enabled for trivially copyable types so std::atomic can be
   * instantiated for ValueType. LockFreeItem will be created if atomic operations are implemented
   * for ValueType. Otherwise, LockBasedItem will be created
   * \tparam ValueType - type of the item
   * \param value - initial value of the item
   * \param writeHeavy - if true, standard mutexes and unique locks will be used (better for write-heavy modifications)
   * if false, shared mutexes and read-write locks will be used (better for read-heavy modifications)
   * has no effect in case LockFreeItem is created
   */
  template <
    typename ValueType, 
    typename std::enable_if_t<
      std::is_trivially_copyable<
        std::decay_t<
          ValueType
        >
      >::value
    >* = nullptr
  >
  std::unique_ptr<Item<std::decay_t<ValueType>>> make_item(const ValueType& value, bool writeHeavy = false);

  /**
   * \brief Creates an item
   * \details This implementation is only enabled for trivially copyable types so std::atomic can be
   * instantiated for ValueType. LockFreeItem will be created if atomic operations are implemented
   * for ValueType. Otherwise, LockBasedItem will be created
   * \tparam ValueType - type of the item
   * \param value - initial value of the item
   * \param writeHeavy - if true, standard mutexes and unique locks will be used (better for write-heavy modifications)
   * if false, shared mutexes and read-write locks will be used (better for read-heavy modifications)
   * has no effect in case LockFreeItem is created
   */
  template <
    typename ValueType, 
    typename std::enable_if_t<
      std::is_trivially_copyable<
        std::decay_t<
          ValueType
        >
      >::value
    >* = nullptr
  >
  std::unique_ptr<Item<std::decay_t<ValueType>>> make_item(ValueType&& value, bool writeHeavy = false);

  /**
   * \brief Creates an item
   * \details This implementation is only enabled for non-trivially copyable types so std::atomic cannot be
   * instantiated for ValueType. LockBasedItem will be created
   * \tparam ValueType - type of the item
   * \param value - initial value of the item
   * \param writeHeavy - if true, standard mutexes and unique locks will be used (better for write-heavy modifications)
   * if false, shared mutexes and read-write locks will be used (better for read-heavy modifications)
   */
  template <
    typename ValueType, 
    typename std::enable_if_t<
      !std::is_trivially_copyable<
        std::decay_t<
          ValueType
        >
      >::value
    >* = nullptr
  >
  std::unique_ptr<Item<std::decay_t<ValueType>>> make_item(const ValueType& value, bool writeHeavy = false);

  /**
   * \brief Creates an item
   * \details This implementation is only enabled for non-trivially copyable types so std::atomic cannot be
   * instantiated for ValueType. LockBasedItem will be created
   * \tparam ValueType - type of the item
   * \param value - initial value of the item
   * \param writeHeavy - if true, standard mutexes and unique locks will be used (better for write-heavy modifications)
   * if false, shared mutexes and read-write locks will be used (better for read-heavy modifications)
   */
  template <
    typename ValueType, 
    typename std::enable_if_t<
      !std::is_trivially_copyable<
        std::decay_t<
          ValueType
        >
      >::value
    >* = nullptr
  >
  std::unique_ptr<Item<std::decay_t<ValueType>>> make_item(ValueType&& value, bool writeHeavy = false);

}

#include <cache/item_factory.hpp>
