#pragma once

#include <memory>

namespace cache
{

  /**
   * \class Item
   * \brief Represents a single element in a cache
   * \details All non-special member functions are threadsafe
   * \tparam ValueType - type of elements in the cache
   */
  template <typename ValueType>
  class Item
  {
  public:
    /**
     * \brief Atomically retrieves the value from the item
     */
    virtual ValueType read() const = 0;

    /**
     * \brief Atomically updates the value in the item
     */
    virtual void update(const ValueType& value) = 0;

    /**
     * \brief Atomically updates the value in the item
     */
    virtual void update(ValueType&& value) = 0;

    /**
     * \brief Atomically updates the value with desired in case it is currently equal to expected
     * \details No updates are made in case the current value is different from expected
     * \param expected - expected current value
     * \param desired - new value to update the item with in case the current value == expected
     */
    virtual void compare_exchange(const ValueType& expected, const ValueType& desired) = 0;

    /**
     * \brief Atomically updates the value with desired in case it is currently equal to expected
     * \details No updates are made in case the current value is different from expected
     * \param expected - expected current value
     * \param desired - new value to update the item with in case the current value == expected
     */
    virtual void compare_exchange(const ValueType& expected, ValueType&& desired) = 0;

    virtual ~Item() = default;
  };

}

