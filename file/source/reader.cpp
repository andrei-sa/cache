#include <reader.h>

#include <utility/exceptions.h>

#include <fstream>

namespace file
{

  class Reader::Impl
  {
  public:
    Impl(const std::string& in, const std::string& out, const Task& task) try
      : m_inPath(in)
      , m_outPath(out)
      , m_in(in)
      , m_out(out, std::ios_base::trunc)
      , m_task(task)
    {
      THROW_IF(!m_in.good(), "Failed to open the reader input file = '", in, "'");
      THROW_IF(!m_out.good(), "Failed to open the reader output file = '", out, "'");
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
      THROW_IF(m_in.fail(), "Failed to read from input file '", m_inPath, "'!");

      size_t key;
      try
      {
        key = std::stoll(str);
      }
      catch (...)
      {
        RETHROW("Failed to convert key string = '", str, "' to an integer!");
      }

      THROW_IF(key == 0, "Key = 0 found in the input file! Item indexing starts fron 1");
      auto result = m_task(key);

      m_out << result << std::endl;
      THROW_IF(m_out.fail(), "Failed to write to input file '", m_outPath, "'!"); 

      return true;
    }
    catch (...)
    {
      RETHROW("Failed to execute reader iteration for input file '", m_inPath
        , "' and output file '", m_outPath, "'!");
    }

  private:
    std::string m_inPath;
    std::string m_outPath;
    std::ifstream m_in;
    std::ofstream m_out;
    Task m_task;
  };

  Reader::Reader(const std::string& in, const std::string& out, const Task& task)
    : m_impl(std::make_unique<Impl>(in, out, task))
  {
  }

  Reader::Reader(Reader&&) noexcept = default;

  Reader& Reader::operator=(Reader&&) noexcept = default;

  bool Reader::operator()()
  {
    return m_impl->operator()();
  }

  Reader::~Reader() = default;

}
