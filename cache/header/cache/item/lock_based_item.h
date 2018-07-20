#pragma once

#include <cache/item.h>
#include <cache/lock_policy.h>

#include <memory>

namespace cache
{

  /**
   * \class LockBasedItem
   * \brief Uses locks to deliver atomic operations
   * \details All non-special member functions are threadsafe
   * \tparam ValueType - type of elements in the cache
   */
  template <typename ValueType>
  class LockBasedItem : public Item<ValueType>
  {
  public:
    /**
     * \brief Constructor
     * \param value - initial value to store in the item
     * \param writeHeavy - if true, standard mutexes and unique locks will be used (better for write-heavy modifications)
     * if false, shared mutexes and read-write locks will be used (better for read-heavy modifications)
     */
    template <typename ValueTypeFwd>
    LockBasedItem(ValueTypeFwd&& value, bool writeHeavy);

    /**
     * \brief Atomically retrieves the value from the item
     */
    virtual ValueType read() const override final;

    /**
     * \brief Atomically updates the value in the item
     */
    virtual void update(const ValueType& value) override final;

    /**
     * \brief Atomically updates the value in the item
     */
    virtual void update(ValueType&& value) override final;

    /**
     * \brief Atomically updates the value with desired in case it is currently equal to expected
     * \details No updates are made in case the current value is different from expected
     * \param expected - expected current value
     * \param desired - new value to update the item with in case the current value == expected
     */
    virtual void compare_exchange(const ValueType& expected, const ValueType& desired) override final;

    /**
     * \brief Atomically updates the value with desired in case it is currently equal to expected
     * \details No updates are made in case the current value is different from expected
     * \param expected - expected current value
     * \param desired - new value to update the item with in case the current value == expected
     */
    virtual void compare_exchange(const ValueType& expected, ValueType&& desired) override final;

  protected:
    ValueType m_value;
    std::unique_ptr<LockPolicy> m_lockPolicy;
  };

}

#include <cache/item/lock_based_item.hpp>
