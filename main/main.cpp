#include <cache/cache.h>

#include <file/item_file.h>
#include <file/reader.h>
#include <file/writer.h>

#include <utility/exceptions.h>

#include <chrono>
#include <fstream>
#include <future>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace
{

  using namespace std::chrono_literals;

  struct Options
  {
    size_t size;
    std::string readers;
    std::string writers;
    std::string items;
    bool writeHeavy;
    bool floatOptimized;
    bool realtimeConsistent;
  };

  bool parse_options(Options& options, int argc, char** argv)
  {
    Options result;

    if (argc < 5 || argc >= 9)
    {
      return false;
    }

    try
    {
      result.size = std::stoll(argv[1]);
    }
    catch (...)
    {
      return false;
    }

    result.readers = argv[2];
    result.writers = argv[3];
    result.items = argv[4];

    result.writeHeavy = false;
    result.floatOptimized = false;
    result.realtimeConsistent = false;

    for (int i = 5; i < argc; ++i)
    {
      std::string option = argv[i];

      if (option == "write_heavy")
      {
        result.writeHeavy = true;
      }
      else if (option == "float_optimized")
      {
        result.floatOptimized = true;
      }
      else if (option == "realtime_consistent")
      {
        result.realtimeConsistent = true;
      }
      else
      {
        return false;
      }
    }

    options = result;

    return true;
  }

  std::tuple<std::vector<file::Reader>, std::vector<file::Writer>> initialize_readers_and_writers(
    const Options& options
  ) try
  {
    auto itemFile = std::make_shared<file::ItemFile>(options.items, options.writeHeavy);
    std::vector<file::Reader> readers;
    std::vector<file::Writer> writers;

    if (options.floatOptimized)
    {
      const float empty = std::numeric_limits<float>::min();
      const float defaultValue = empty + std::numeric_limits<float>::epsilon();

      auto updateItemFile = [itemFile, empty, defaultValue] (size_t key, float value)
      {
        THROW_IF(key == 0, "Invalid key == 0!");

        if (value != defaultValue)
        {
          itemFile->write_line(key - 1, value == empty ? std::string() : std::to_string(value));
        }
      };

      auto cache = options.realtimeConsistent ?
      std::make_shared<cache::Cache<size_t, float>>(
        options.size, 
        [] (size_t, float) noexcept {},
        options.writeHeavy,
        defaultValue
      ) :  
      std::make_shared<cache::Cache<size_t, float>>(
        options.size, 
        [updateItemFile] (size_t key, float value) noexcept
        {
          try
          {
            updateItemFile(key, value);
          }
          catch (const std::exception& e)
          {
            std::cerr << "Exception in update hook!" << std::endl;

            utility::print_exception(e);
          }
          catch (...)
          {
            std::cerr << "Unknown exception in update hook!" << std::endl;
          }
        },
        options.writeHeavy,
        defaultValue
      );  

      {
        std::fstream in(options.readers);
        THROW_IF(!in.good(), "Failed to open the reader file = '", options.readers, "'!");

        std::string buff;
        while (std::getline(in, buff))
        {
          THROW_IF(in.fail(), "Failed to read fron the reader file = '", options.readers, "'!");

          readers.emplace_back(buff, buff + ".out", [cache, itemFile, empty, defaultValue] (size_t key)
          {
            auto ptr = (*cache)[key];

            auto value = ptr->read();

            if (value == defaultValue)
            {
              THROW_IF(key == 0, "Invalid key == 0!");
              auto newValueStr = itemFile->read_line(key - 1);

              if (newValueStr.empty())
              {
                value = empty;
              }
              else
              {
                try
                {
                  value = std::stof(newValueStr);
                }
                catch (...)
                {
                  RETHROW("Failed to convert value = '", newValueStr, "' to float! Disable float optimization to proceed");
                }
              }

              ptr->compare_exchange(defaultValue, value);

              return newValueStr + " Disk";
            }

            return std::to_string(value) + " Cache";
          });
        }
      }
      {
        std::fstream in(options.writers);
        THROW_IF(!in.good(), "Failed to open the writer file = '", options.writers, "'!");

        std::string buff;
        while (std::getline(in, buff))
        {
          THROW_IF(in.fail(), "Failed to read from the writer file = '", options.writers, "'!");

          writers.emplace_back(buff, [cache, updateItemFile, realtimeConsistent = options.realtimeConsistent] 
            (size_t key, const std::string& valueStr)
          {
            float value;

            try
            {
              value = std::stof(valueStr);
            }
            catch (...)
            {
              RETHROW("Failed to convert value = '", valueStr, "' to float! Disable float optimization to proceed");
            }

            (*cache)[key]->update(value);

            if (realtimeConsistent)
            {
              updateItemFile(key, value);
            }
          });
        }
      }
    }
    else
    {
      const std::string defaultValue = "NODATA";

      auto updateItemFile = [itemFile, defaultValue] (size_t key, const std::string& value)
      {
        THROW_IF(key == 0, "Invalid key == 0!");

        if (value != defaultValue)
        {
          itemFile->write_line(key - 1, value);
        }
      };

      auto cache = options.realtimeConsistent ?
      std::make_shared<cache::Cache<size_t, std::string>>(
        options.size, 
        [] (size_t, std::string) noexcept {},
        options.writeHeavy,
        defaultValue
      ) :  
      std::make_shared<cache::Cache<size_t, std::string>>(
        options.size, 
        [updateItemFile] (size_t key, const std::string& value) noexcept
        {
          try
          {
            updateItemFile(key, value);
          }
          catch (const std::exception& e)
          {
            std::cerr << "Exception in update hook!" << std::endl;

            utility::print_exception(e);
          }
          catch (...)
          {
            std::cerr << "Unknown exception in update hook!" << std::endl;
          }
        },
        options.writeHeavy,
        defaultValue
      );  

      {
        std::fstream in(options.readers);
        THROW_IF(!in.good(), "Failed to open the reader file = '", options.readers, "'!");

        std::string buff;
        while (std::getline(in, buff))
        {
          THROW_IF(in.fail(), "Failed to read fron the reader file = '", options.readers, "'!");

          readers.emplace_back(buff, buff + ".out", [cache, itemFile, defaultValue] (size_t key)
          {
            auto ptr = (*cache)[key];
            auto value = ptr->read();

            if (value == defaultValue)
            {
              THROW_IF(key == 0, "Invalid key == 0!");
              value = itemFile->read_line(key - 1);

              ptr->compare_exchange(defaultValue, value);

              return value + " Disk";
            }

            return value + " Cache";
          });
        }
      }
      {
        std::fstream in(options.writers);
        THROW_IF(!in.good(), "Failed to open the writer file = '", options.writers, "'!");

        std::string buff;
        while (std::getline(in, buff))
        {
          THROW_IF(in.fail(), "Failed to read from the writer file = '", options.writers, "'!");

          writers.emplace_back(buff, [cache, updateItemFile, realtimeConsistent = options.realtimeConsistent] 
            (size_t key, const std::string& value)
          {
            (*cache)[key]->update(value);

            if (realtimeConsistent)
            {
              updateItemFile(key, value);
            }
          });
        }
      }
    }

    return { std::move(readers), std::move(writers) };
  }
  catch (...)
  {
    RETHROW("Failed to initialize readers and writers!");
  }

}

int main(int argc, char** argv) try
{
  Options options;
  if (!parse_options(options, argc, argv))
  {
    std::cout << "Invalid arguments! Use: " 
              << argv[0] 
              << " <size_of_cache> <reader_file> <writer_file> <items_file>"
              << " <write_heavy/read_heavy (optional; default = read_heavy)>"
              << " <float_optimized (optional)>"
              << " <realtime_consistent (optional)>"
              << std::endl;

    return 1;
  }

  auto readersAndWriters = initialize_readers_and_writers(options);

  auto& readers = std::get<0>(readersAndWriters);
  auto& writers = std::get<1>(readersAndWriters);

  std::atomic<bool> stopFlag;
  stopFlag.store(false);

  std::promise<void> startPromise;
  auto startSignal = startPromise.get_future().share();

  std::list<std::future<void>> readerFutures;
  for (auto& reader : readers)
  {
    readerFutures.push_back(
      std::async(
        std::launch::async, 
        [reader = std::move(reader), startSignal, &stopFlag] () mutable
        {
          startSignal.get();

          while (!stopFlag.load() && reader());
        }
      )
    );
  }

  std::list<std::future<void>> writerFutures;
  for (auto& writer : writers)
  {
    writerFutures.push_back(
      std::async(
        std::launch::async, 
        [writer = std::move(writer), startSignal, &stopFlag] () mutable
        {
          startSignal.get();

          while (!stopFlag.load() && writer());
        }
      )
    );
  }

  startPromise.set_value();

  while (!readerFutures.empty() && !writerFutures.empty()) try
  {
    readerFutures.remove_if([] (auto& future)
    {
      auto status = future.wait_for(1ms);

      if (status == std::future_status::ready)
      {
        try
        {
          future.get();
        }
        catch (...)
        {
          RETHROW("Exception raised in a reader thread!");
        }

        return true;
      }

      return false;
    });

    writerFutures.remove_if([] (auto& future)
    {
      auto status = future.wait_for(1ms);

      if (status == std::future_status::ready)
      {
        try
        {
          future.get();
        }
        catch (...)
        {
          RETHROW("Exception raised in a writer thread!");
        }

        return true;
      }

      return false;
    });
  }
  catch (...)
  {
    stopFlag = true;

    for (const auto& reader : readerFutures)
    {
      reader.wait();
    }

    for (const auto& writer : writerFutures)
    {
      writer.wait();
    }

    throw;
  }

}
catch (const std::exception& e)
{
  utility::print_exception(e);
}
