#include "measurement/dbsflushcache.hpp"

tuddbs::dbsCPUCacheFlusher::dbsCPUCacheFlusher()
: dev_null("/dev/null"),
  rng_shuffle(std::random_device{}()),
  rng_positions(std::random_device{}()),
  cache_element_count{2*dbsCPU::get_instance().get_accumulated_cache_size_in_bytes() / sizeof(uint64_t)},
  dist_positions{0, cache_element_count - 1},
  complete_cache_buffer{new uint64_t[cache_element_count]}
{
  if (!dev_null.is_open()) {
    std::cerr << "Could not open /dev/null for writing." << std::endl;
    exit(-20);
  }
  dist_t dist{0, 1000};
  for (uint64_t i = 0; i < cache_element_count; i++) {
    complete_cache_buffer[i] = i;
  }
}

tuddbs::dbsCPUCacheFlusher::~dbsCPUCacheFlusher() {
  dev_null.close();
  delete[] complete_cache_buffer;
}

void tuddbs::dbsCPUCacheFlusher::flush() {
  // Shuffle the buffer
  std::shuffle(complete_cache_buffer, complete_cache_buffer + cache_element_count, rng_shuffle);
  // Write 100 random positions from buffer to /dev/null to ensure, that this is not optimized out
  for (size_t i = 0; i < 100; ++i) {
    dev_null.write(reinterpret_cast<char*>(&complete_cache_buffer[dist_positions(rng_positions)]), sizeof(uint64_t));
  }
}

