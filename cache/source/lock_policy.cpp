#include <lock_policy.h>
#include <lock_policy/read_heavy_lock_policy.h>
#include <lock_policy/write_heavy_lock_policy.h>

namespace cache
{

  std::unique_ptr<LockPolicy> make_lock_policy(bool writeHeavy)
  {
    return writeHeavy
         ? std::unique_ptr<LockPolicy>(new WriteHeavyLockPolicy())
         : std::unique_ptr<LockPolicy>(new ReadHeavyLockPolicy());
  }

}
