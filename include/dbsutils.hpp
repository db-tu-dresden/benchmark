#ifndef TUDDBS_BENCHMARK_INCLUDE_DBSUTILS_HPP
#define TUDDBS_BENCHMARK_INCLUDE_DBSUTILS_HPP

#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

namespace tuddbs {

  using kibi = std::ratio<1024, 1>;
  using mebi = std::ratio<1024*1024, 1>;
  using gibi = std::ratio<1024*1024*1024, 1>;


  std::string now_format_str();
}

#endif //TUDDBS_BENCHMARK_INCLUDE_DBSUTILS_HPP
