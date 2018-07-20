#include <file/reader.h>

#include <gtest/gtest.h>

#include <fstream>

namespace
{

  using namespace file;

  class ReaderFixture : protected Reader
                      , public ::testing::Test
  {
  protected:
    ReaderFixture()
      : Reader(copy_test_file("test_reader.dat"), "test_reader_out.dat", [this] (size_t key)
        {
          return m_task(key);
        })
    {
    }

  private:
    std::string copy_test_file(const std::string& path) const
    {
      auto newPath = path + ".test";
      std::ofstream out(newPath, std::ios_base::trunc);

      std::ifstream in(path);
      
      std::string buffer;
      while (std::getline(in, buffer))
      {
        out << buffer << std::endl;
      }

      return newPath;
    }

  protected:
    Task m_task;
  };

  TEST(ReaderTests, InvalidInputFile)
  {
    EXPECT_ANY_THROW(Reader("test_reader_invalid.dat", "test_reader_out.dat", [] (size_t) { return ""; }));
  }

  TEST_F(ReaderFixture, ReadAll)
  {
    int counter = 0;
    m_task = [&counter] (size_t key)
    {
      switch (counter)
      {
      case 0:
        EXPECT_EQ(1, key);
        break;
      case 1:
        EXPECT_EQ(3, key);
        break;
      case 2:
        EXPECT_EQ(5, key);
        break;
      case 3:
        EXPECT_EQ(7, key);
        break;
      case 4:
        EXPECT_EQ(1, key);
        break;
      case 5:
        EXPECT_EQ(3, key);
        break;
      case 6:
        EXPECT_EQ(5, key);
        break;
      case 7:
        EXPECT_EQ(7, key);
        break;
      default:
        EXPECT_TRUE(false);
        break;
      }

      ++counter;

      return "";
    };

    for (int i = 0; i < 8; ++i)
    {
      ASSERT_TRUE(operator()());
    }

    EXPECT_EQ(8, counter);

    EXPECT_FALSE(operator()());
    EXPECT_EQ(8, counter);

    EXPECT_FALSE(operator()());
    EXPECT_EQ(8, counter);
  } 

  TEST_F(ReaderFixture, Output)
  {
    m_task = [] (size_t key)
    {
      return std::to_string(key * 5);
    };

    while (operator()());

    std::ifstream in("test_reader_out.dat");
    ASSERT_TRUE(in.good());

    std::string buff;
    for (int i = 0; i < 2; ++i)
    {
      std::getline(in, buff);
      ASSERT_TRUE(in.good());
      EXPECT_EQ("5", buff);

      std::getline(in, buff);
      ASSERT_TRUE(in.good());
      EXPECT_EQ("15", buff);

      std::getline(in, buff);
      ASSERT_TRUE(in.good());
      EXPECT_EQ("25", buff);

      std::getline(in, buff);
      ASSERT_TRUE(in.good());
      EXPECT_EQ("35", buff);
    }

    std::getline(in, buff);
    EXPECT_TRUE(in.eof());
  } 

}
