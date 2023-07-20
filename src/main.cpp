#include "dbsbenchmark.hpp"

int main() {

  using namespace tuddbs;
  cpu_flush_t flusher;
  csv_filewriter_t writer("test", "\t");

  auto fields = writer.create_field_names_description();
  fields.append(dbsCPU::get_instance());
  fields.append_bulk("duration", "tput");
  writer.set_field_names(fields);
  writer.append(dbsCPU::get_instance());

  auto start = default_clock_t::now();
  flusher.flush();
  auto end = default_clock_t::now();

  
  writer.append_bulk(
    default_clock_t::duration(start, end), 
    default_tput_t::throughput(start, end, flusher.get_cache_element_size_in_bytes())
  );


  return 0;
}