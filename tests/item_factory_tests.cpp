#include <cache/item_factory.h>
#include <cache/item/lock_based_item.h>
#include <cache/item/lock_free_item.h>

#include <gtest/gtest.h>

#include <type_traits>
#include <vector>

namespace
{

  using namespace cache;

  TEST(ItemFactoryTests, Int)
  {
    auto value = 1;
    auto item = make_item(value);
    EXPECT_FALSE(nullptr == dynamic_cast<LockFreeItem<std::decay_t<decltype(value)>>*>(item.get()) 
              && nullptr == dynamic_cast<LockBasedItem<std::decay_t<decltype(value)>>*>(item.get()));
    EXPECT_EQ(1, item->read());
  }

  TEST(ItemFactoryTests, ConstCharArray)
  {
    auto value = "abc";
    auto item = make_item(value);
    EXPECT_FALSE(nullptr == dynamic_cast<LockFreeItem<std::decay_t<decltype(value)>>*>(item.get()) 
              && nullptr == dynamic_cast<LockBasedItem<std::decay_t<decltype(value)>>*>(item.get()));
    EXPECT_EQ("abc", item->read());
  }

  TEST(ItemFactoryTests, Double)
  {
    auto value = 124.325;
    auto item = make_item(value);
    EXPECT_FALSE(nullptr == dynamic_cast<LockFreeItem<std::decay_t<decltype(value)>>*>(item.get()) 
              && nullptr == dynamic_cast<LockBasedItem<std::decay_t<decltype(value)>>*>(item.get()));
    EXPECT_EQ(124.325, item->read());
  }

  TEST(ItemFactoryTests, Float)
  {
    auto value = -0.f;
    auto item = make_item(value);
    EXPECT_FALSE(nullptr == dynamic_cast<LockFreeItem<std::decay_t<decltype(value)>>*>(item.get()) 
              && nullptr == dynamic_cast<LockBasedItem<std::decay_t<decltype(value)>>*>(item.get()));
    EXPECT_EQ(-0.f, item->read());
  }

  TEST(ItemFactoryTests, String)
  {
    std::string value = "abc";
    auto item = make_item(value);
    EXPECT_FALSE(nullptr == dynamic_cast<LockBasedItem<std::decay_t<decltype(value)>>*>(item.get()));
    EXPECT_EQ("abc", item->read());
  }

  TEST(ItemFactoryTests, Vector)
  {
    auto value = std::vector<int> { 1, 3, 2 };
    auto item = make_item(value);
    EXPECT_FALSE(nullptr == dynamic_cast<LockBasedItem<std::decay_t<decltype(value)>>*>(item.get()));
    auto result = item->read();
    ASSERT_EQ(3, result.size());
    EXPECT_EQ(1, result[0]);
    EXPECT_EQ(3, result[1]);
    EXPECT_EQ(2, result[2]);
  }

}
