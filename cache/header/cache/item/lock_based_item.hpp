#pragma once

namespace cache
{

  template <typename ValueType>
  template <typename ValueTypeFwd>
  LockBasedItem<ValueType>::LockBasedItem(ValueTypeFwd&& value, bool writeHeavy)
    : m_value(std::forward<ValueTypeFwd>(value))
    , m_lockPolicy(make_lock_policy(writeHeavy))
  {
  }

  template <typename ValueType>
  ValueType LockBasedItem<ValueType>::read() const
  {
    auto lock = m_lockPolicy->acquire_shared_lock();

    return m_value;
  }

  template <typename ValueType>
  void LockBasedItem<ValueType>::update(const ValueType& value)
  {
    auto lock = m_lockPolicy->acquire_unique_lock();

    m_value = value;
  }

  template <typename ValueType>
  void LockBasedItem<ValueType>::update(ValueType&& value)
  {
    auto lock = m_lockPolicy->acquire_unique_lock();

    m_value = std::move(value);
  }

  template <typename ValueType>
  void LockBasedItem<ValueType>::compare_exchange(const ValueType& expected, const ValueType& desired)
  {
    auto lock = m_lockPolicy->acquire_unique_lock();

    if (m_value == expected)
    {
      m_value = desired;
    }
  }

  template <typename ValueType>
  void LockBasedItem<ValueType>::compare_exchange(const ValueType& expected, ValueType&& desired)
  {
    auto lock = m_lockPolicy->acquire_unique_lock();

    if (m_value == expected)
    {
      m_value = std::move(desired);
    }
  }

}
