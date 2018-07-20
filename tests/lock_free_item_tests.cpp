#include <cache/item/lock_free_item.h>

#include <gtest/gtest.h>

#include <future>
#include <thread>
#include <unordered_set>
#include <vector>

namespace
{

  using namespace cache;

  class LockFreeItemFixture : protected LockFreeItem<float>
                            , public ::testing::Test
  {
  public:
    LockFreeItemFixture()
      : LockFreeItem<float>(0.f)
    {
    }
  };

  TEST_F(LockFreeItemFixture, UpdateAndReadST)
  {
    EXPECT_EQ(0.f, read());
    EXPECT_EQ(0.f, read());

    update(1.f);
    EXPECT_EQ(1.f, read());
    EXPECT_EQ(1.f, read());

    update(.5f);
    EXPECT_EQ(.5f, read());
    EXPECT_EQ(.5f, read());

    update(61235.6f);
    EXPECT_EQ(61235.6f, read());
    EXPECT_EQ(61235.6f, read());

    update(-61235.6f);
    EXPECT_EQ(-61235.6f, read());
    EXPECT_EQ(-61235.6f, read());

    update(0.f);
    EXPECT_EQ(0.f, read());
    EXPECT_EQ(0.f, read());
  }

  TEST_F(LockFreeItemFixture, SingleValueMT)
  {
    update(1.f);

    std::promise<void> promise;
    auto signal = promise.get_future().share();

    std::vector<std::thread> threads;
    threads.reserve(20);
    
    for (int i = 0; i < 20; ++i)
    {
      threads.emplace_back([this, signal]
      {
        signal.wait();

        for (int i = 0; i < 10000; ++i)
        {
          ASSERT_EQ(1.f, read());
          update(1.f);
          ASSERT_EQ(1.f, read());
        }
      });
    }

    promise.set_value();

    for (auto& thread : threads)
    {
      thread.join();
    }
  }

  TEST_F(LockFreeItemFixture, TwoValuesMT)
  {
    update(1.f);

    std::promise<void> promise;
    auto signal = promise.get_future().share();

    std::vector<std::thread> threads;
    threads.reserve(20);
    
    for (int i = 0; i < 20; ++i)
    {
      threads.emplace_back([this, signal]
      {
        signal.wait();

        for (int i = 0; i < 10000; ++i)
        {
          {
            auto value = read();
            ASSERT_TRUE(1.f == value || -2.f == value) << "Actual: << " << value;
          }

          update(rand() % 2 == 0 ? 1.f : -2.f);

          {
            auto value = read();
            ASSERT_TRUE(1.f == value || -2.f == value) << "Actual: << " << value;
          }
        }
      });
    }

    promise.set_value();

    for (auto& thread : threads)
    {
      thread.join();
    }
  }

  TEST_F(LockFreeItemFixture, DictionaryMT)
  {
    const static int size = 1000;

    std::vector<float> sourceDictionary;
    std::unordered_set<float> searchDictionary;
    sourceDictionary.reserve(size);
    searchDictionary.reserve(size);
    for (int i = 0; i < size; ++i)
    {
      auto value = 1.f * (rand() % size) + .001f * (rand() % size);
      sourceDictionary.push_back(value);
      searchDictionary.insert(value);
    }

    update(sourceDictionary[0]);

    std::promise<void> promise;
    auto signal = promise.get_future().share();

    std::vector<std::thread> threads;
    threads.reserve(20);
    
    for (int i = 0; i < 20; ++i)
    {
      threads.emplace_back([this, &sourceDictionary, &searchDictionary, signal]
      {
        signal.wait();

        for (int i = 0; i < 10000; ++i)
        {
          {
            auto value = read();
            auto iter = searchDictionary.find(value);
            ASSERT_TRUE(iter != searchDictionary.end()) << "Actual: << " << value;
          }

          {
            update(sourceDictionary[rand() % sourceDictionary.size()]);
          }

          {
            auto value = read();
            auto iter = searchDictionary.find(value);
            ASSERT_TRUE(iter != searchDictionary.end()) << "Actual: << " << value;
          }
        }
      });
    }

    promise.set_value();

    for (auto& thread : threads)
    {
      thread.join();
    }
  }

}
