#pragma once

#include <cache/item/lock_based_item.h>
#include <cache/item/lock_free_item.h>

#include <atomic>

namespace cache
{

  template <typename ValueType, typename std::enable_if_t<std::is_trivially_copyable<std::decay_t<ValueType>>::value>*>
  std::unique_ptr<Item<std::decay_t<ValueType>>> make_item(const ValueType& value, bool writeHeavy)
  {
    if (std::atomic<std::decay_t<ValueType>>().is_lock_free())
    {
      return std::make_unique<LockFreeItem<std::decay_t<ValueType>>>(value);
    }
    else
    {
      return std::make_unique<LockBasedItem<std::decay_t<ValueType>>>(value, writeHeavy);
    }
  }

  template <typename ValueType, typename std::enable_if_t<std::is_trivially_copyable<std::decay_t<ValueType>>::value>*>
  std::unique_ptr<Item<std::decay_t<ValueType>>> make_item(ValueType&& value, bool writeHeavy)
  {
    if (std::atomic<std::decay_t<ValueType>>().is_lock_free())
    {
      return std::make_unique<LockFreeItem<std::decay_t<ValueType>>>(std::move(value));
    }
    else
    {
      return std::make_unique<LockBasedItem<std::decay_t<ValueType>>>(std::move(value), writeHeavy);
    }
  }

  template <typename ValueType, typename std::enable_if_t<!std::is_trivially_copyable<std::decay_t<ValueType>>::value>*>
  std::unique_ptr<Item<std::decay_t<ValueType>>> make_item(const ValueType& value, bool writeHeavy)
  {
    return std::make_unique<LockBasedItem<std::decay_t<ValueType>>>(value, writeHeavy);
  }

  template <typename ValueType, typename std::enable_if_t<!std::is_trivially_copyable<std::decay_t<ValueType>>::value>*>
  std::unique_ptr<Item<std::decay_t<ValueType>>> make_item(ValueType&& value, bool writeHeavy)
  {
    return std::make_unique<LockBasedItem<std::decay_t<ValueType>>>(std::move(value), writeHeavy);
  }

}
