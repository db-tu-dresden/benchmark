#ifndef TUDDBS_BENCHMARK_INCLUDE_HARDWARE_DBSMEMORY_HPP
#define TUDDBS_BENCHMARK_INCLUDE_HARDWARE_DBSMEMORY_HPP

#include <numeric>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include <dirent.h>

#include "../filewriter/dbsserialize.hpp"

namespace tuddbs {

  class dbsCPU : public dbsSerializable {
    private:
      std::string cpu_name;
      std::vector<size_t> cache_sizes;
    public:
      std::string get_cpu_name() const {
        return cpu_name;
      }
      size_t get_cpu_cache_size(int level) const {
        return cache_sizes[level];
      }
      size_t get_cache_level_count() const {
        return cache_sizes.size();
      }
      size_t get_accumulated_cache_size_in_bytes() const {
        return std::accumulate(cache_sizes.begin(), cache_sizes.end(), 0);
      }
    private:
      void determine_cpu_name();
      void determine_cache_sizes();
    public:
      virtual ~dbsCPU() = default;
    private:
      dbsCPU(): cpu_name("Unknown CPU"), cache_sizes() {
        determine_cpu_name();
        determine_cache_sizes();        
      }
      
    public:
      static dbsCPU get_instance() {
        static dbsCPU instance;
        return instance;
      }

    public:
      std::vector<std::pair<std::string, std::string>> serialize() const override;
        
  };

}

#endif //TUDDBS_BENCHMARK_INCLUDE_HARDWARE_DBSMEMORY_HPP