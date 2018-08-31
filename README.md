## C++14 implementation of multi-threaded LRU cache

This repository contains an implementation of multi-threaded LRU cache using C++14 only. The implementation is assembled into the 'cache' library, which is dynamically linked against the 'utility' library. One may opt for static linkage instead to prevent name conflicts with other utility libraries.

'file' library and 'main' executable are used to test the cache using a set of items stored in a file, and a number of writers and readers each running on a separate thread. Please note that the 'file' library is for evaluation purposes only. In most real cases, using a thread per writer/reader approach is sub-optimal and event-based multi-threading is preferred.

The cache can be useful in massive multi-threaded environments when memory used by an in-memory cache is crucial, and/or where each read/write operation introduces more overhead than thread synchronization. In other cases, single-threaded caches will generally outperform the multi-threaded implementation.



### Documentation

Further documentation can be found in the doc/ folder

For information on how to build the program, check doc/build.txt 

For information on how to run the program, check doc/run.txt 

For certain design considerations, check doc/design.txt

For information on how to build the Doxygen code documentation, check
https://www.stack.nl/~dimitri/doxygen/manual/starting.html
