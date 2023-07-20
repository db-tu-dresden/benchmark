#ifndef TUDDBS_BENCHMARK_INCLUDE_MEASUREMENT_DBSFLUSHCACHE_HPP
#define TUDDBS_BENCHMARK_INCLUDE_MEASUREMENT_DBSFLUSHCACHE_HPP

#include <fstream>
#include <iostream>
#include <random>
#include <chrono>
#include <cstdint>
#include <cstddef>
#include "../hardware/dbscpu.hpp"


namespace tuddbs {
  class dbsCPUCacheFlusher {
    public:
      using rng_t = std::mt19937;
      using dist_t = std::uniform_int_distribution<rng_t::result_type>;
    private:
      std::ofstream dev_null;
      rng_t rng_shuffle;
      rng_t rng_positions;
      size_t cache_element_count;
      dist_t dist_positions;
      uint64_t * complete_cache_buffer;
      
    public:
      virtual ~dbsCPUCacheFlusher();
      dbsCPUCacheFlusher() ;
      size_t get_cache_element_count() const {
        return cache_element_count;
      }
      size_t get_cache_element_size_in_bytes() const {
        return sizeof(uint64_t);
      }
    public:
      void flush();
  };


  using cpu_flush_t = dbsCPUCacheFlusher;
}

#endif //TUDDBS_BENCHMARK_INCLUDE_MEASUREMENT_DBSFLUSHCACHE_HPP