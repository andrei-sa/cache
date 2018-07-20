#include <file/item_file.h>

#include <gtest/gtest.h>

#include <fstream>

namespace
{

  using namespace file;

  class ItemFileFixture : protected ItemFile
                        , public ::testing::Test
  {
  protected:
    ItemFileFixture()
      : ItemFile(copy_test_file("test_item_file.dat"), false)
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
  };

  TEST(ItemFileTests, InvalidFile)
  {
    EXPECT_ANY_THROW(ItemFile("test_item_file_invalid.dat", false));
    EXPECT_ANY_THROW(ItemFile("test_item_file_invalid.dat", false));
  }

  TEST_F(ItemFileFixture, Read)
  {
    EXPECT_EQ("0", read_line(0));
    EXPECT_EQ("0", read_line(0));

    EXPECT_EQ("-33", read_line(2));
    EXPECT_EQ("-33", read_line(2));

    EXPECT_EQ("", read_line(3));
    EXPECT_EQ("", read_line(3));

    EXPECT_EQ("90.0", read_line(6));
    EXPECT_EQ("90.0", read_line(6));

    EXPECT_EQ("10000.0", read_line(11));
    EXPECT_EQ("10000.0", read_line(11));

    EXPECT_EQ("-33", read_line(2));
    EXPECT_EQ("-33", read_line(2));
  } 

  TEST_F(ItemFileFixture, ReadOutOfBound)
  {
    EXPECT_ANY_THROW(read_line(12));
    EXPECT_ANY_THROW(read_line(12));

    EXPECT_ANY_THROW(read_line(13));
    EXPECT_ANY_THROW(read_line(13));

    EXPECT_ANY_THROW(read_line(100));
    EXPECT_ANY_THROW(read_line(100));
  }

  TEST_F(ItemFileFixture, Write)
  {
    write_line(0, "50.1");
    EXPECT_EQ("50.1", read_line(0));

    write_line(5, "abc");
    EXPECT_EQ("abc", read_line(5));

    write_line(9, "1");
    EXPECT_EQ("1", read_line(9));

    write_line(11, "");
    EXPECT_EQ("", read_line(11));

    write_line(11, "def");
    EXPECT_EQ("def", read_line(11));

    write_line(5, "11.1");
    EXPECT_EQ("11.1", read_line(5));

    write_line(1, "");
    EXPECT_EQ("", read_line(1));

    EXPECT_EQ("50.1", read_line(0));
    EXPECT_EQ("11.1", read_line(5));
    EXPECT_EQ("1", read_line(9));
    EXPECT_EQ("def", read_line(11));
    EXPECT_EQ("", read_line(1));
    EXPECT_EQ("-33", read_line(2));
    EXPECT_EQ("75.2", read_line(4));
  }

  TEST_F(ItemFileFixture, WriteOutOfBound)
  {
    EXPECT_ANY_THROW(write_line(12, "abc"));
    EXPECT_ANY_THROW(write_line(12, "abc"));

    EXPECT_ANY_THROW(write_line(13, ""));
    EXPECT_ANY_THROW(write_line(13, ""));

    EXPECT_ANY_THROW(write_line(100, "11.1"));
    EXPECT_ANY_THROW(write_line(100, "11.1"));

    EXPECT_ANY_THROW(read_line(12));
    EXPECT_ANY_THROW(read_line(13));
    EXPECT_ANY_THROW(read_line(100));
  }

}
