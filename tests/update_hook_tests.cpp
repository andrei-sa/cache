#include <cache/update_hook.h>

#include <gtest/gtest.h>

namespace
{

  using namespace cache;

  struct TestFuncObj
  {
    void operator()(const std::string& key, const int& value) const noexcept
    {
      EXPECT_EQ("bcd", key);
      EXPECT_EQ(-5, value);
    }
  };

  TEST(UpdateHookTests, Closure)
  {
    auto closure = [] (const double& key, const std::string& value) noexcept
    {
      EXPECT_EQ(1.5, key);
      EXPECT_EQ("abc", value);
    };

    auto hook = make_update_hook<double, std::string>(std::move(closure));

    hook(1.5, "abc");
  }

  TEST(UpdateHookTests, FunctionObject)
  {
    auto hook = make_update_hook<std::string, int>(TestFuncObj());

    hook("bcd", -5);
  }

}
