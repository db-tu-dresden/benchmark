#ifndef TUDDBS_BENCHMARK_INCLUDE_MEASUREMENT_DBSTIME_HPP
#define TUDDBS_BENCHMARK_INCLUDE_MEASUREMENT_DBSTIME_HPP

#include <chrono>
#include <ratio>
#include <type_traits>
#include <string>

namespace tuddbs{
  template<typename Ratio = std::chrono::nanoseconds, typename Clock = std::chrono::high_resolution_clock>
    class dbsClock { 
      public:
        using rep_t = typename Ratio::rep;
        using period_t = typename Ratio::period;
        using duration_t = typename std::chrono::duration<rep_t, period_t>;
        using time_point_t = typename std::chrono::time_point<Clock, duration_t>;

      public:
        /**
         * @brief Get the current time point.
         * @return time_point_t 
         */
        static auto now() -> time_point_t {
          return std::chrono::time_point_cast<duration_t>(Clock::now());
        }
        /**
         * @brief Get the current time point as a rep_t.
         * @param tp The time point to convert.
         * @return rep_t 
         */
        static auto to_rep(time_point_t const & tp) -> rep_t {
          return std::chrono::time_point_cast<duration_t>(tp).time_since_epoch().count();
        }
        static std::string unit_str() {
          if constexpr (std::is_same_v<period_t, std::nano>) {
            return "ns";
          } else if constexpr (std::is_same_v<period_t, std::micro>) {
            return "us";
          } else if constexpr (std::is_same_v<period_t, std::milli>) {
            return "ms";
          } else if constexpr (std::is_same_v<period_t, std::ratio<1>>) {
            return "s";
          } else if constexpr (std::is_same_v<period_t, std::ratio<60>>) {
            return "min";
          } else if constexpr (std::is_same_v<period_t, std::ratio<3600>>) {
            return "h";
          } else {
            return "unknown";
          }
        }
        /**
         * @brief Get the current time point as a duration_t.
         * @tparam T The type of the duration to convert to.
         * @param start 
         * @param end 
         * @return auto 
         */
        template<typename T = void>
        static auto duration(time_point_t const & start, time_point_t const & end) {
          if constexpr (std::is_same_v<T, void>) {
            return std::chrono::duration_cast<Ratio>(end - start).count();
          } else {
            return static_cast<T>(std::chrono::duration_cast<Ratio>(end - start).count());
          }
        }
        template<typename T = void>
        static std::string duration_str(time_point_t const & start, time_point_t const & end) {
          return std::to_string(duration<T>(start, end));
        }
        template<typename T = void>
        static std::string duration_fully_qualified_str(time_point_t const & start, time_point_t const & end) {
          auto time_str = duration_str<T>(start, end);
          return time_str + " " + unit_str();
        }
        template<typename T = void>
        static auto duration_normalized(time_point_t const & start, time_point_t const & end) -> double{
          return duration<T>(start, end) * period_t::num / (period_t::den);
        }
    };

  using default_clock_t = tuddbs::dbsClock<>;
} // namespace tuddbs
#endif //TUDDBS_BENCHMARK_INCLUDE_MEASUREMENT_DBSTIME_HPP