#include <cache/cache.h>

#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <future>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace
{

  using namespace cache;

  class CacheFixture : protected Cache<int, std::string>
                     , public ::testing::Test
  {
  protected:
    CacheFixture()
      : Cache<int, std::string>(
          5, 
          [this] (const int& key, const std::string& value) noexcept
          {
            try
            {
              m_hook(key, value);
            }
            catch (...)
            {
            }
          },
          false
        )
      , m_hook([] (int, const std::string&)
        {
        })
    {
    }

  protected:
    std::function<void(int, const std::string&)> m_hook;
  };

  TEST_F(CacheFixture, RetrieveEmpty)
  {
    auto ptr = operator[](5);
    EXPECT_EQ("", ptr->read());

    ptr = operator[](5);
    EXPECT_EQ("", ptr->read());

    ptr = operator[](20);
    EXPECT_EQ("", ptr->read());

    ptr = operator[](-5);
    EXPECT_EQ("", ptr->read());

    ptr = operator[](0);
    EXPECT_EQ("", ptr->read());
  } 

  TEST_F(CacheFixture, RetrieveStored)
  {
    auto ptr = operator[](5);
    ptr->update("abc");
    ptr = operator[](5);
    EXPECT_EQ("abc", ptr->read());

    ptr = operator[](3);
    ptr->update("bcd");
    ptr = operator[](3);
    EXPECT_EQ("bcd", ptr->read());

    ptr = operator[](-2);
    ptr->update("cde");
    ptr = operator[](-2);
    EXPECT_EQ("cde", ptr->read());

    ptr = operator[](0);
    ptr->update("defb");
    ptr = operator[](0);
    EXPECT_EQ("defb", ptr->read());

    ptr = operator[](1000);
    ptr->update("");
    ptr = operator[](1000);
    EXPECT_EQ("", ptr->read());
  }

  TEST_F(CacheFixture, ModifyStored)
  {
    auto ptr = operator[](5);
    ptr->update("abc");
    ptr = operator[](5);
    EXPECT_EQ("abc", ptr->read());
    ptr->update("qwe");
    ptr = operator[](5);
    EXPECT_EQ("qwe", ptr->read());

    ptr = operator[](3);
    ptr->update("bcd");
    ptr = operator[](3);
    EXPECT_EQ("bcd", ptr->read());
    ptr->update("wert");
    ptr = operator[](3);
    EXPECT_EQ("wert", ptr->read());

    ptr = operator[](-2);
    ptr->update("cde");
    ptr = operator[](-2);
    EXPECT_EQ("cde", ptr->read());
    ptr->update("ty");
    ptr = operator[](-2);
    EXPECT_EQ("ty", ptr->read());

    ptr = operator[](0);
    ptr->update("defb");
    ptr = operator[](0);
    EXPECT_EQ("defb", ptr->read());
    ptr->update("");
    ptr = operator[](0);
    EXPECT_EQ("", ptr->read());

    ptr = operator[](1000);
    ptr->update("");
    ptr = operator[](1000);
    EXPECT_EQ("", ptr->read());
    ptr->update("as");
    ptr = operator[](1000);
    EXPECT_EQ("as", ptr->read());
  }

  TEST_F(CacheFixture, OldRemoval)
  {
    auto ptr = operator[](5);
    ptr->update("abc");

    ptr = operator[](6);
    ptr->update("bce");

    ptr = operator[](20);
    ptr->update("cdef");

    ptr = operator[](-5);
    ptr->update("");

    ptr = operator[](0);
    ptr->update("qwe");

    bool called;

    called = false;
    m_hook = [&called] (int key, const std::string& value)
    {
      EXPECT_EQ(5, key);
      EXPECT_EQ("abc", value);
      called = true;
    };
    ptr = operator[](11);
    ptr->update("asdf");
    EXPECT_TRUE(called);

    called = false;
    m_hook = [&called] (int key, const std::string& value)
    {
      EXPECT_EQ(6, key);
      EXPECT_EQ("bce", value);
      called = true;
    };
    ptr = operator[](5);
    EXPECT_TRUE(called);
    EXPECT_EQ("", ptr->read());
    
    called = false;
    m_hook = [&called] (int key, const std::string& value)
    {
      EXPECT_EQ(20, key);
      EXPECT_EQ("cdef", value);
      called = true;
    };
    ptr = operator[](6);
    EXPECT_TRUE(called);
    EXPECT_EQ("", ptr->read());
    
    called = false;
    m_hook = [&called] (int key, const std::string& value)
    {
      EXPECT_EQ(-5, key);
      EXPECT_EQ("", value);
      called = true;
    };
    ptr = operator[](20);
    EXPECT_TRUE(called);
    EXPECT_EQ("", ptr->read());
    
    called = false;
    m_hook = [&called] (int key, const std::string& value)
    {
      EXPECT_EQ(0, key);
      EXPECT_EQ("qwe", value);
      called = true;
    };
    ptr = operator[](-5);
    EXPECT_TRUE(called);
    EXPECT_EQ("", ptr->read());
    
    called = false;
    m_hook = [&called] (int key, const std::string& value)
    {
      EXPECT_EQ(11, key);
      EXPECT_EQ("asdf", value);
      called = true;
    };
    ptr = operator[](30);
    EXPECT_EQ("", ptr->read());
    
    m_hook = [] (int key, const std::string&) {};
  }

  TEST_F(CacheFixture, Actualize)
  {
    auto ptr = operator[](5);
    ptr->update("abc");

    ptr = operator[](6);
    ptr->update("bce");

    ptr = operator[](20);
    ptr->update("cdef");

    ptr = operator[](-5);
    ptr->update("");

    ptr = operator[](0);
    ptr->update("qwe");

    ptr = operator[](6);
    ptr->update("jkl");

    bool called;

    called = false;
    m_hook = [&called] (int key, const std::string& value)
    {
      EXPECT_EQ(5, key);
      EXPECT_EQ("abc", value);
      called = true;
    };
    ptr = operator[](11);
    ptr->update("asdf");
    EXPECT_TRUE(called);

    called = false;
    m_hook = [&called] (int key, const std::string& value)
    {
      EXPECT_EQ(20, key);
      EXPECT_EQ("cdef", value);
      called = true;
    };
    ptr = operator[](5);
    EXPECT_TRUE(called);
    EXPECT_EQ("", ptr->read());
    ptr->update("bnm");
    
    called = false;
    m_hook = [&called] (int key, const std::string& value)
    {
      EXPECT_EQ(-5, key);
      EXPECT_EQ("", value);
      called = true;
    };
    ptr = operator[](20);
    EXPECT_TRUE(called);
    EXPECT_EQ("", ptr->read());

    ptr = operator[](11);
    
    called = false;
    m_hook = [&called] (int key, const std::string& value)
    {
      EXPECT_EQ(0, key);
      EXPECT_EQ("qwe", value);
      called = true;
    };
    ptr = operator[](-5);
    EXPECT_TRUE(called);
    EXPECT_EQ("", ptr->read());
    
    called = false;
    m_hook = [&called] (int key, const std::string& value)
    {
      EXPECT_EQ(6, key);
      EXPECT_EQ("jkl", value);
      called = true;
    };
    ptr = operator[](30);
    EXPECT_EQ("", ptr->read());
    
    called = false;
    m_hook = [&called] (int key, const std::string& value)
    {
      EXPECT_EQ(5, key);
      EXPECT_EQ("bnm", value);
      called = true;
    };
    ptr = operator[](50);
    EXPECT_EQ("", ptr->read());
    
    m_hook = [] (int key, const std::string&) {};
  }

  TEST(CacheTests, UpdateHookOnDestructor)
  {
    int counter = 0;

    { 
      std::function<void(const int& key, const std::string& value)> func = [] (const int&, const std::string&) {};

      Cache<int, std::string> cache(
        5, 
        [&func] (const int& key, const std::string& value) noexcept
        {
          func(key, value);
        },
        false
      );

      auto ptr = cache[5];
      ptr->update("abc");

      ptr = cache[6];
      ptr->update("bce");

      ptr = cache[20];
      ptr->update("cdef");

      ptr = cache[-5];
      ptr->update("");

      ptr = cache[0];
      ptr->update("qwe");

      ptr = cache[6];
      ptr->update("sdf");

      ptr = cache[7];
      ptr->update("jkl");

      func = [this, &counter] (const int& key, const std::string& value) noexcept
      {
        switch (key)
        {
        case 6:
          EXPECT_EQ("sdf", value);
          break;
        case 20:
          EXPECT_EQ("cdef", value);
          break;
        case -5:
          EXPECT_EQ("", value);
          break;
        case 0:
          EXPECT_EQ("qwe", value);
          break;
        case 7:
          EXPECT_EQ("jkl", value);
          break;
        default:
          EXPECT_TRUE(false);
          break;
        }

        ++counter;
      };
    }

    EXPECT_EQ(5, counter);
  }

  TEST_F(CacheFixture, DictionaryMT)
  {
    std::unordered_map<int, std::unordered_set<std::string>> dictionary = 
    {
      { 2, { "abc", "fgwae", "asf" } },
      { -3, { "gdas", "bstrhs", "gergher", "berse" } },
      { 7, { "asf", "wgweg" } },
      { 8, { "lwefg", " aff wef", "fkweof", "fweofkowef", "a" } },
      { 134, { "mvergvgae" } },
      { -2354, { "wetwe", "greherg", "gkoerkga", "faeogaog" } },
      { 0, { "asfasfga", "geksro", "gkreogksea" } }
    };

    std::vector<int> keys(dictionary.size());
    std::transform(dictionary.begin(), dictionary.end(), keys.begin(), [] (const auto& e)
    {
      return e.first;
    });

    std::promise<void> promise;
    auto signal = promise.get_future().share();

    std::vector<std::future<void>> futures;
    futures.reserve(20);

    for (int i = 0; i < 20; ++i)
    {
      futures.push_back(std::async(std::launch::async, [this, &dictionary, &keys, signal]
      {
        signal.wait();

        for (int i = 0; i < 10000; ++i)
        {
          auto key = keys[rand() % keys.size()];

          auto ptr = operator[](key);
          ASSERT_NE(nullptr, ptr) << "key = " << key ;

          auto value = ptr->read();
          auto iter = dictionary[key].find(value);
          EXPECT_TRUE(iter != dictionary[key].end() || value == "") << "key = " << key << ", value = '" << value << "'";

          auto newIter = dictionary[key].begin();
          std::advance(newIter, rand() % dictionary[key].size());
          auto newValue = *newIter;

          ptr->update(newValue); 
        }
      }));
    }

    promise.set_value();

    for (auto& future : futures)
    {
      EXPECT_NO_THROW(future.get());
    }
  }
    
  TEST(CacheTests, DefaultValue)
  {
    std::function<void(const int& key, const std::string& value)> func = [] (const int&, const std::string&) {};

    Cache<int, std::string> cache(
      5, 
      [&func] (const int& key, const std::string& value) noexcept
      {
        func(key, value);
      },
      false,
      "default"
    );

    auto ptr = cache[5];
    EXPECT_EQ("default", ptr->read());
    ptr->update("abc");

    ptr = cache[7];
    EXPECT_EQ("default", ptr->read());
    ptr->update("cde");

    ptr = cache[20];
    EXPECT_EQ("default", ptr->read());

    ptr = cache[-5];
    EXPECT_EQ("default", ptr->read());

    ptr = cache[0];
    EXPECT_EQ("default", ptr->read());

    ptr = cache[4];
    EXPECT_EQ("default", ptr->read());
    
    ptr = cache[5];
    EXPECT_EQ("default", ptr->read());

    ptr = cache[7];
    EXPECT_EQ("default", ptr->read());
  }

}
