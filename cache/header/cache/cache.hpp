#pragma once

#include <utility/exceptions.h>

namespace cache
{

  template <typename KeyType, typename ValueType>
  template <typename UpdateHookFwd>
  Cache<KeyType, ValueType>::Cache(
    size_t size, 
    UpdateHookFwd&& updateHook, 
    bool writeHeavy,
    const ValueType& defaultValue
  ) try
    : m_size(size)
    , m_updateHook(std::forward<UpdateHookFwd>(updateHook))
    , m_writeHeavy(writeHeavy)
    , m_defaultValue(defaultValue)
  {
    THROW_IF(m_size == 0, "Attempt to create a Cache with size = 0!");

    m_itemMap.reserve(m_size);
  }
  catch (...)
  {
    RETHROW("Failed to create a ", (writeHeavy ? "write heavy" : "read heavy"), " Cache of size = ", size);
  }

  template <typename KeyType, typename ValueType>
  typename Cache<KeyType, ValueType>::ItemPtr Cache<KeyType, ValueType>::operator[](const KeyType& key) try
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto mapIter = m_itemMap.find(key);
    if (mapIter != m_itemMap.end())
    {
      auto queueIter = mapIter->second;
      THROW_IF(queueIter == m_itemQueue.end(), "Queue iterator for key = ", key, " is out of bound!");
      THROW_IF(queueIter->key != key, "Keys are inconsistent between the queue and the map! Map key = ", key, ", queue key = ", queueIter->key);

      m_itemQueue.push_front(*queueIter);
      mapIter->second = m_itemQueue.begin();
      m_itemQueue.erase(queueIter);

      return m_itemQueue.front().item;
    } 

    if (m_itemQueue.size() == m_size)
    {
      remove_latest();
    }

    auto ptr = add(key);

    return ptr;
  }
  catch (...)
  {
    RETHROW("Failed to access key = ", key, " in the cache!");
  }

  template <typename KeyType, typename ValueType>
  void Cache<KeyType, ValueType>::remove_latest() try
  {
    auto latest = m_itemQueue.back();
    const auto& key = latest.key;

    auto mapIter = m_itemMap.find(key);
    THROW_IF(mapIter == m_itemMap.end(), "Keys are inconsistent between the queue and the map! Latest key = "
      , key, " in the queue is not found in the map!");

    m_itemQueue.pop_back();
    m_itemMap.erase(mapIter);
  }
  catch (...)
  {
    RETHROW("Failed to remove the latest element in the item queue of size = ", m_itemQueue.size());
  }
  
  template <typename KeyType, typename ValueType>
  typename Cache<KeyType, ValueType>::ItemPtr Cache<KeyType, ValueType>::add(const KeyType& key)
  {
    auto item = make_item<ValueType>(m_defaultValue, m_writeHeavy);
    auto sharedItem = std::shared_ptr<Item<ValueType>>(
      item.get(), 
      [key, updateHook = m_updateHook] (Item<ValueType>* item)
      {
        updateHook(key, item->read());
        free(item);
      }
    );
    item.release();

    m_itemQueue.push_front({ key, std::move(sharedItem) });
    auto front = m_itemQueue.front();

    m_itemMap.emplace(key, m_itemQueue.begin());

    return front.item;
  }

}
