#include <writer.h>

#include <utility/exceptions.h>

#include <fstream>

namespace file
{

  class Writer::Impl
  {
  public:
    Impl(const std::string& in, const Task& task) try
      : m_inPath(in)
      , m_in(in)
      , m_task(task)
    {
      THROW_IF(!m_in.good(), "Failed to open the reader input file = '", in, "'");
    }
    catch (...)
    {
      RETHROW("Failed to construct a reader!");
    }

    bool operator()() try
    {
      std::string str;
      std::getline(m_in, str);
      if (m_in.eof())
      {
        return false;
      }
      THROW_IF(m_in.fail(), "Failed to read from input file!");

      size_t key;
      std::string value;
      try
      {
        auto pos = str.find(" ");
        THROW_IF(pos == std::string::npos, "No key-value delimiter (whitespace) found!");
        auto keyStr = str.substr(0, pos);
        key = std::stoll(str);

        ++pos;
        THROW_IF(pos >= str.size(), "No value found!");
        value = str.substr(pos);
      }
      catch (...)
      {
        RETHROW("Failed to convert key string = '", str, "' to an integer!");
      }

      THROW_IF(key == 0, "Key = 0 found in the input file! Item indexing starts fron 1");
      m_task(key, std::move(value));

      return true;
    }
    catch (...)
    {
      RETHROW("Failed to execute writer iteration for file '", m_inPath, "'!");
    }

  private:
    std::string m_inPath;
    std::ifstream m_in;
    Task m_task;
  };

  Writer::Writer(const std::string& in, const Task& task)
    : m_impl(std::make_unique<Impl>(in, task))
  {
  }

  Writer::Writer(Writer&&) noexcept = default;

  Writer& Writer::operator=(Writer&&) noexcept = default;

  bool Writer::operator()()
  {
    return m_impl->operator()();
  }

  Writer::~Writer() = default;

}
