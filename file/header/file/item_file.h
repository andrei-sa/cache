#pragma once

#include <memory>
#include <string>

namespace file
{

  /**
   * \class ItemFile
   * \brief Interface to an item file
   * \details All non-special member functions are threadsafe
   */
  class ItemFile
  {
  private:
    class Impl;

  public:
    /**
     * \brief Constructor
     * \param path - path to the item file
     * \param writeHeavy - if true, WriteHeavyLockPolicy will be for file access
     * \param if false, ReadHeavyLockPolicy will be used for file access
     * \throw if path cannot be opened
     */
    ItemFile(const std::string& path, bool writeHeavy);

    /**
     * \brief Constructor
     * \param path - path to the item file
     * \param writeHeavy - if true, WriteHeavyLockPolicy will be for file access
     * \param if false, ReadHeavyLockPolicy will be used for file access
     * \throw if path cannot be opened
     */
    ItemFile(std::string&& path, bool writeHeavy);

    /**
     * \brief Implementation-file-defined default move constructor
     * \details Used to enable incomplete type move construction
     */
    ItemFile(ItemFile&&) noexcept;

    /**
     * \brief Implementation-file-defined default move assignment operator
     * \details Used to enable incomplete type move assignment
     */
    ItemFile& operator=(ItemFile&&) noexcept;

    /**
     * \brief Reads the line of a specified number from the item file
     * \throw if the file read has failed
     */
    std::string read_line(size_t num) const;

    /**
     * \brief Writes a string to the line of a specified number in the item file
     * \details Since line length is not fixed, the entire file will be rewritten using a temporary file
     * \param num - line number
     * \param str - string to write
     * \throw if the file write has failed
     */
    void write_line(size_t num, const std::string& str) const;

    /**
     * \brief Writes a string to the line of a specified number in the item file
     * \details Since line length is not fixed, the entire file will be rewritten using a temporary file
     * \param num - line number
     * \param str - string to write
     * \throw if the file write has failed
     */
    void write_line(size_t num, std::string&& str) const; 

    /**
     * \brief Implementation-file-defined default destruction
     * \details Used to enable incomplete type destruction
     */
    ~ItemFile();

  private:
    std::unique_ptr<Impl> m_impl;
  };

}
