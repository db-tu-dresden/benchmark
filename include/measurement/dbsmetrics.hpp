#ifndef TUDDBS_BENCHMARK_INCLUDE_MEASUREMENT_DBSMETRICS_HPP
#define TUDDBS_BENCHMARK_INCLUDE_MEASUREMENT_DBSMETRICS_HPP

#include "dbstime.hpp"
#include "../dbsutils.hpp"

namespace tuddbs {
  template<typename DataSizeRatio = gibi, typename ClockT = dbsClock<>>
    class dbsThroughput { 
      public:
        static std::string unit_str() {
          if constexpr (std::is_same_v<DataSizeRatio, std::ratio<1,1>>) {
            return "B/s";
          } else if constexpr (std::is_same_v<DataSizeRatio, std::kilo>) {
            return "KB/s";
          } else if constexpr (std::is_same_v<DataSizeRatio, kibi>) {
            return "KiB/s";
          } else if constexpr (std::is_same_v<DataSizeRatio, std::mega>) {
            return "MB/s";
          } else if constexpr (std::is_same_v<DataSizeRatio, mebi>) {
            return "MiB/s";
          } else if constexpr (std::is_same_v<DataSizeRatio, std::giga>) {
            return "GB/s";
          } else if constexpr (std::is_same_v<DataSizeRatio, gibi>) {
            return "GiB/s";
          } else {
            return "unknown";
          }
          
        }
        auto static throughput(typename ClockT::time_point_t const & start, typename ClockT::time_point_t const & end, std::size_t const size_in_bytes) -> double {
          auto const size = static_cast<double>(size_in_bytes) * static_cast<double>(DataSizeRatio::den) / static_cast<double>(DataSizeRatio::num);
          return size / ClockT::template duration_normalized<double>(start, end);
        }

        static std::string throughput_str(typename ClockT::time_point_t const & start, typename ClockT::time_point_t const & end, std::size_t const size_in_bytes) {
          return std::to_string(throughput(start, end, size_in_bytes));
        }

        static std::string throughput_fully_qualified_str(typename ClockT::time_point_t const & start, typename ClockT::time_point_t const & end, std::size_t const size_in_bytes) {
          auto time_str = throughput_str(start, end);
          return time_str + " " + unit_str();
        }
    };

  using default_tput_t = tuddbs::dbsThroughput<>;
}
#endif //TUDDBS_BENCHMARK_INCLUDE_MEASUREMENT_DBSMETRICS_HPP 