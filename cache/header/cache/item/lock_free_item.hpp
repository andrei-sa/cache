#pragma once

namespace cache
{

  template <typename ValueType>
  template <typename ValueTypeFwd>
  LockFreeItem<ValueType>::LockFreeItem(ValueTypeFwd&& value)
  {
    update(std::forward<ValueTypeFwd>(value));
  }

  template <typename ValueType>
  ValueType LockFreeItem<ValueType>::read() const
  {
    return m_value.load();
  }

  template <typename ValueType>
  void LockFreeItem<ValueType>::update(const ValueType& value)
  {
    m_value.store(value);
  }

  template <typename ValueType>
  void LockFreeItem<ValueType>::update(ValueType&& value)
  {
    m_value.store(std::move(value));
  }

  template <typename ValueType>
  void LockFreeItem<ValueType>::compare_exchange(const ValueType& expected, const ValueType& desired)
  {
    auto expectedAdaptor = expected;

    m_value.compare_exchange_strong(expectedAdaptor, desired);
  }

  template <typename ValueType>
  void LockFreeItem<ValueType>::compare_exchange(const ValueType& expected, ValueType&& desired)
  {
    auto expectedAdaptor = expected;
    
    m_value.compare_exchange_strong(expectedAdaptor, desired);
  }

}
