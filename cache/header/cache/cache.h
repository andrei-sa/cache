#pragma once

#include <cache/item_factory.h>
#include <cache/lock_policy.h>
#include <cache/update_hook.h>

#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace cache
{

  /**
   * \class Cache
   * \brief Least-recently used cache
   * \tparam KeyType - type of keys used for object referencing
   * \tparam ValueType - type of stored objects
   */
  template <typename KeyType, typename ValueType>
  class Cache
  {
  public:
    /**
     * \class ItemPtr
     * \brief Shared pointer to an item
     */
    using ItemPtr = std::shared_ptr<Item<ValueType>>;

  private:
    struct ItemInfo
    {
      KeyType key;
      ItemPtr item;
    };

    using ItemQueue = std::list<ItemInfo>;
    using ItemIter = typename ItemQueue::iterator;
    using ItemMap = std::unordered_map<KeyType, ItemIter>;

  public:
    /**
     * \brief Constructor
     * \details updateHook is executed on destruction of the last pointer to each item
     * \param size - size (in objects) of the cache
     * \param updateHook - function object satisfying UpdateHook requirements
     * \param writeHeavy - if true, WriteHeavyLockPolicy will be used in items
     * \param if false, ReadHeavyLockPolicy will be used in items
     * \param defaultValue - value stored in an item until it is first written
     */
    template <typename UpdateHookFwd>
    Cache(
      size_t size, 
      UpdateHookFwd&& updateHook, 
      bool writeHeavy = false,
      const ValueType& defaultValue = ValueType()
    );

    /**
     * \brief Returns a shared pointer to the item for a given key
     * \details If an item exists for the key, it will be moved to the back of the remove queue
     * and a pointer to it will be returned.
     * If the item does not exist, a new item will be created in the back of the remove queue
     * and a pointer to it will be returned. 
     * Shared pointers can be safely used throughout their lifetime as the lifetime of the
     * pointer item will be extended by the shared pointer.
     */
    ItemPtr operator[](const KeyType& key); 
    
  private:
    void remove_latest();
    ItemPtr add(const KeyType& key);

  private:
    const size_t m_size;
    const UpdateHook<KeyType, ValueType> m_updateHook;
    const bool m_writeHeavy;
    const ValueType m_defaultValue;
    ItemQueue m_itemQueue;
    ItemMap m_itemMap;
    std::mutex m_mutex;
  };

}

#include <cache/cache.hpp>
