#include <cache/lock_policy.h>

#include <mutex>

namespace cache
{

  /**
   * \class WriteHeavyUniqueLock
   * \brief Interface to a unique lock constructed from a standard mutex
   */
  class WriteHeavyUniqueLock : public UniqueLock
  {
  public:
    /**
     * \class Constructor
     * \param mutex - mutex to create a lock on
     */
    explicit WriteHeavyUniqueLock(std::mutex& mutex);

  private:
    const std::lock_guard<std::mutex> m_lock;
  };

  /**
   * \class WriteHeavySharedLock
   * \brief Interface to a unique lock constructed from a standard mutex
   */
  class WriteHeavySharedLock : public SharedLock
  {
  public:
    /**
     * \class Constructor
     * \param mutex - mutex to create a lock on
     */
    explicit WriteHeavySharedLock(std::mutex& mutex);

  private:
    const std::lock_guard<std::mutex> m_lock;
  };

  /**
   * \class WriteHeavyLockPolicy
   * \brief Uses a standard mutex to create locks on
   * \details Preferable when lock use is write-heavy
   */
  class WriteHeavyLockPolicy : public LockPolicy
  {
  public:
    /**
     * \brief Creates a UniqueLock on the internal mutex
     */
    virtual std::unique_ptr<UniqueLock> acquire_unique_lock() override final;

    /**
     * \brief Creates a SharedLock on the internal mutex
     */
    virtual std::unique_ptr<SharedLock> acquire_shared_lock() override final;

  private:
    std::mutex m_mutex;
  }; 
  
}
