#include <item_file.h>

#include <cache/lock_policy.h>

#include <utility/exceptions.h>

#include <fstream>
#include <type_traits>

#include <stdio.h>

namespace file
{

  class ItemFile::Impl
  {
  private:
    using LockPolicy = cache::LockPolicy;

  public:
    template <
      typename ItemFilePathFwd,
      typename std::enable_if_t<
        !std::is_base_of<
          ItemFile::Impl,
          std::decay_t<ItemFilePathFwd>
        >::value
      >* = nullptr
    >
    Impl(ItemFilePathFwd&& path, bool writeHeavy) try
      : m_path(std::forward<ItemFilePathFwd>(path))
      , m_lockPolicy(cache::make_lock_policy(writeHeavy))
    {
      std::ifstream in(m_path);

      THROW_IF(in.eof(), "File = ", m_path, " is empty!");
      THROW_IF(in.fail(), "Could not open file = ", m_path);
    }
    catch (...)
    {
      RETHROW("Failed to construct an ItemFile!");
    }

    std::string read_line(size_t num) const try
    {
      auto lock = m_lockPolicy->acquire_shared_lock();

      std::ifstream in(m_path);
      THROW_IF(!in.good(), "Could not open file = ", m_path);

      std::string str;

      for (size_t i = 0; i <= num; ++i)
      {
        std::getline(in, str);

        THROW_IF(!in.good(), "Failed to read line ", i, " from the input file! End of file is "
          , (in.eof() ? "reached" : "not reached"));
      }

      return std::move(str);
    }
    catch (...)
    {
      RETHROW("Failed to read line number = ", num, " of the ItemFile!");
    }

    template <typename StrFwd>
    void write_line(size_t num, StrFwd&& str) const
    {
      auto tempPath = m_path + ".tmp";

      auto lock = m_lockPolicy->acquire_unique_lock();

      std::ofstream out(tempPath, std::ios_base::trunc);
      THROW_IF(out.fail(), "Failed to open a temporary file = ", tempPath);

      try
      {
        std::ifstream in(m_path);
        THROW_IF(!in.good(), "Could not open file = ", m_path);

        std::string buffer;

        for (size_t i = 0; i < num; ++i)
        {
          std::getline(in, buffer);

          THROW_IF(!in.good(), "Failed to read line ", i, " from the input file! End of file is "
            , (in.eof() ? "reached" : "not reached"));

          out << buffer << std::endl;
        }

        std::getline(in, buffer);

        THROW_IF(!in.good(), "Failed to read line ", num, " from the input file! End of file is "
          , (in.eof() ? "reached" : "not reached"));

        out << std::forward<StrFwd>(str) << std::endl;

        while (std::getline(in, buffer))
        {
          out << buffer << std::endl;
        }
      }
      catch (...)
      {
        remove(m_path.c_str());

        throw;
      }

      auto result = remove(m_path.c_str());
      THROW_IF(result != 0, "Failed to remove file = ", m_path);

      result = rename(tempPath.c_str(), m_path.c_str());
      THROW_IF(result != 0, "Failed to rename temporary file '", tempPath, "' to '", m_path.c_str(), "'");
    }

  private:
    std::string m_path;
    std::unique_ptr<LockPolicy> m_lockPolicy;
  };

  ItemFile::ItemFile(const std::string& path, bool writeHeavy)
    : m_impl(std::make_unique<Impl>(path, writeHeavy))
  {
  }

  ItemFile::ItemFile(std::string&& path, bool writeHeavy)
    : m_impl(std::make_unique<Impl>(std::move(path), writeHeavy))
  {
  }

  ItemFile::ItemFile(ItemFile&&) noexcept = default;

  ItemFile& ItemFile::operator=(ItemFile&&) noexcept = default;

  std::string ItemFile::read_line(size_t num) const
  {
    return m_impl->read_line(num);
  } 

  void ItemFile::write_line(size_t num, const std::string& str) const
  {
    return m_impl->write_line(num, str);
  }

  void ItemFile::write_line(size_t num, std::string&& str) const
  {
    return m_impl->write_line(num, std::move(str));
  }

  ItemFile::~ItemFile() = default;

}
