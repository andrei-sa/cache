#pragma once

#include <functional>
#include <memory>
#include <string>

namespace file
{

  /**
   * \class Writer
   * \brief Reads item keys and values from the input file and executes a write task
   */
  class Writer
  {
  private:
    class Impl;

  public:
    /**
     * \class Task
     * \brief Task to be executed on the input data
     */
    using Task = std::function<void(size_t, std::string&&)>;

  public:
    /**
     * \brief Constructor
     * \param in - path to the input file
     * \param task - data write task to be executed on each key-value pair read from the input file
     * \throw if in cannot be opened
     */
    Writer(const std::string& in, const Task& task);

    /**
     * \brief Implementation-file-defined default move constructor
     * \details Used to enable incomplete type move construction
     */
    Writer(Writer&&) noexcept;

    /**
     * \brief Implementation-file-defined default move assignment operator
     * \details Used to enable incomplete type move assignment
     */
    Writer& operator=(Writer&&) noexcept;

    /**
     * \brief Reads the next line of the input file and executes the task on the parsed key-value pair
     * \return true if a line was read and the task was executed
     * \return false if the end of the file had been reached
     * \throw if the input file read has failed
     */
    bool operator()();

    /**
     * \brief Implementation-file-defined default destruction
     * \details Used to enable incomplete type destruction
     */
    ~Writer();

  private:
    std::unique_ptr<Impl> m_impl;
  };

}
