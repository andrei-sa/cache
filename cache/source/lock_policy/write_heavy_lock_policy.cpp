#include <lock_policy/write_heavy_lock_policy.h>

namespace cache
{

  WriteHeavyUniqueLock::WriteHeavyUniqueLock(std::mutex& mutex)
    : m_lock(mutex)
  {
  }

  WriteHeavySharedLock::WriteHeavySharedLock(std::mutex& mutex)
    : m_lock(mutex)
  {
  }

  std::unique_ptr<UniqueLock> WriteHeavyLockPolicy::acquire_unique_lock()
  {
    return std::unique_ptr<UniqueLock>(new WriteHeavyUniqueLock(m_mutex));
  }
  
  std::unique_ptr<SharedLock> WriteHeavyLockPolicy::acquire_shared_lock()
  {
    return std::unique_ptr<SharedLock>(new WriteHeavySharedLock(m_mutex));
  }

}
