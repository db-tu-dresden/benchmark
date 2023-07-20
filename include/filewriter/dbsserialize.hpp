#ifndef TUDDBS_BENCHMARK_INCLUDE_FILEWRITER_DBSSERIALIZE_HPP
#define TUDDBS_BENCHMARK_INCLUDE_FILEWRITER_DBSSERIALIZE_HPP

#include <vector>
#include <string>
#include <tuple>

namespace tuddbs {
  struct dbsSerializable {
    virtual std::vector<std::pair<std::string, std::string>> serialize() const = 0;
  };
}


#endif