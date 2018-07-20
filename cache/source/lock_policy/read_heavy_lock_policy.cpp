#include <lock_policy/read_heavy_lock_policy.h>

#include <mutex>

namespace cache
{

  ReadHeavyUniqueLock::ReadHeavyUniqueLock(utility::SharedMutex& mutex)
    : m_lock(mutex)
  {
  }

  ReadHeavySharedLock::ReadHeavySharedLock(utility::SharedMutex& mutex)
    : m_lock(mutex)
  {
  }

  std::unique_ptr<UniqueLock> ReadHeavyLockPolicy::acquire_unique_lock()
  {
    return std::unique_ptr<UniqueLock>(new ReadHeavyUniqueLock(m_mutex));
  }
  
  std::unique_ptr<SharedLock> ReadHeavyLockPolicy::acquire_shared_lock()
  {
    return std::unique_ptr<SharedLock>(new ReadHeavySharedLock(m_mutex));
  }

}
