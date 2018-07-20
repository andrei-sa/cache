#include <cache/lock_policy.h>

#include <utility/shared_mutex_adaptor.h>

namespace cache
{

  /**
   * \class ReadHeavyUniqueLock
   * \brief Interface to a unique lock constructed from a shared mutex
   */
  class ReadHeavyUniqueLock : public UniqueLock
  {
  public:
    /**
     * \class Constructor
     * \param mutex - mutex to create a lock on
     */
    explicit ReadHeavyUniqueLock(utility::SharedMutex& mutex);

  private:
    const std::unique_lock<utility::SharedMutex> m_lock;
  };

  /**
   * \class ReadHeavySharedLock
   * \brief Interface to a shared lock constructed from a shared mutex
   */
  class ReadHeavySharedLock : public SharedLock
  {
  public:
    /**
     * \class Constructor
     * \param mutex - mutex to create a lock on
     */
    explicit ReadHeavySharedLock(utility::SharedMutex& mutex);

  private:
    const std::shared_lock<utility::SharedMutex> m_lock;
  };

  /**
   * \class ReadHeavyLockPolicy
   * \brief Uses a shared mutex to create locks on
   * \details Preferable when lock use is read-heavy
   */
  class ReadHeavyLockPolicy : public LockPolicy
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
    utility::SharedMutex m_mutex;
  }; 
  
}
