#pragma once

#include <memory>

namespace cache
{

  /**
   * \class UniqueLock
   * \brief Interface to classes used for unique locking
   */
  class UniqueLock
  {
  protected:
    UniqueLock() = default;

  public:
    virtual ~UniqueLock() = default;
  };

  /**
   * \class SharedLock
   * \brief Interface to classes used for shared locking
   */
  class SharedLock
  {
  protected:
    SharedLock() = default;

  public:
    virtual ~SharedLock() = default;
  };

  /**
   * \class LockPolicy
   * \briefs Interface to synchronization objects used for read-write locking
   */
  class LockPolicy
  {
  public:
    /**
     * \brief Creates a UniqueLock on the internal mutex
     */
    virtual std::unique_ptr<UniqueLock> acquire_unique_lock() = 0;

    /**
     * \brief Creates a SharedLock on the internal mutex
     */
    virtual std::unique_ptr<SharedLock> acquire_shared_lock() = 0;

    virtual ~LockPolicy() = default;
  };

  /**
   * \brief Creates a LockPolicy
   * \param writeHeavy - if true, standard mutexes and unique locks will be used (better for write-heavy modifications)
   * if false, shared mutexes and true read-write locks will be used (better for read-heavy modifications)
   */
  std::unique_ptr<LockPolicy> make_lock_policy(bool writeHeavy);

}
