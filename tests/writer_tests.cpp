#include <file/writer.h>

#include <gtest/gtest.h>

#include <fstream>

namespace
{

  using namespace file;

  class WriterFixture : protected Writer
                      , public ::testing::Test
  {
  protected:
    WriterFixture()
      : Writer(copy_test_file("test_writer.dat"), [this] (size_t key, std::string&& value)
        {
          return m_task(key, std::move(value));
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

  TEST(WriterTests, InvalidInputFile)
  {
    EXPECT_ANY_THROW(Writer("test_writer_invalid.dat", [] (size_t, std::string&&) {}));
  }

  TEST_F(WriterFixture, ExecuteAll)
  {
    int counter = 0;
    m_task = [&counter] (size_t key, std::string&& value)
    {
      switch (counter)
      {
      case 0:
        EXPECT_EQ(1, key);
        EXPECT_EQ("100", std::move(value));
        break;
      case 1:
        EXPECT_EQ(2, key);
        EXPECT_EQ("200", std::move(value));
        break;
      case 2:
        EXPECT_EQ(4, key);
        EXPECT_EQ("300", std::move(value));
        break;
      case 3:
        EXPECT_EQ(5, key);
        EXPECT_EQ("500", std::move(value));
        break;
      case 4:
        EXPECT_EQ(8, key);
        EXPECT_EQ("800", std::move(value));
        break;
      default:
        EXPECT_TRUE(false);
        break;
      }

      ++counter;
    };

    for (int i = 0; i < 5; ++i)
    {
      ASSERT_TRUE(operator()());
    }

    EXPECT_EQ(5, counter);

    EXPECT_FALSE(operator()());
    EXPECT_EQ(5, counter);

    EXPECT_FALSE(operator()());
    EXPECT_EQ(5, counter);
  } 

}
