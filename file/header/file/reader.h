#pragma once

#include <functional>
#include <memory>
#include <string>

namespace file
{

  /**
   * \class Reader
   * \brief Reads item keys from the input file, executes a read task, and writes the result to the output file
   */
  class Reader
  {
  private:
    class Impl;

  public:
    /**
     * \class Task
     * \brief Task to be executed on the input data
     */
    using Task = std::function<std::string(size_t)>;

  public:
    /**
     * \brief Constructor
     * \param in - path to the input file
     * \param out - path to the output file
     * \param task - data read task to be executed on each key read from the input file
     * \throw if in or out cannot be opened
     */
    Reader(const std::string& in, const std::string& out, const Task& task);

    /**
     * \brief Implementation-file-defined default move constructor
     * \details Used to enable incomplete type move construction
     */
    Reader(Reader&&) noexcept;

    /**
     * \brief Implementation-file-defined default move assignment operator
     * \details Used to enable incomplete type move assignment
     */
    Reader& operator=(Reader&&) noexcept;

    /**
     * \brief Reads the next line of the input file and executes the task on the parsed key
     * \return true if a line was read and the task was executed
     * \return false if the end of the file had been reached
     * \throw if the input file read or the output file write have failed
     */
    bool operator()();

    /**
     * \brief Implementation-file-defined default destruction
     * \details Used to enable incomplete type destruction
     */
    ~Reader();

  private:
    std::unique_ptr<Impl> m_impl;
  };

}
