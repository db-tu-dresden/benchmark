#include "filewriter/dbscsv_filewriter.hpp"

void tuddbs::dbsCSVHeader::append(tuddbs::dbsSerializable const & serializable) {
  auto const serialized = serializable.serialize();
  this->header_field_count += serialized.size();
  for (auto const & [key, value] : serialized) {
    if (this->header.empty()) {
      this->header += key;
    } else {
      this->header += separator + key;
    }
  }
}
std::string tuddbs::dbsCSVFileWriter::get_latest_header() const {
  std::ifstream file(filename);
  bool found_header = false;
  std::string result = "";
  if (file.is_open()) {
    std::string line;
    auto const target = current_header.get_string_id();
    while (std::getline(file, line)) {
      if (target.compare(line) == 0) { // Check if the line starts with "#!--Header"
        found_header = true;
      } else {
        if (found_header) {
          result = line;
          found_header = false;
        }
      }
    }
    file.close();
  } else {
    std::cerr << "Could not open file \"" << filename << ".\" for reading." << std::endl;
    exit(-20); 
  }
  return result;
}

void tuddbs::dbsCSVFileWriter::set_field_names(tuddbs::dbsCSVHeader header) {
  auto current_header_str = this->get_latest_header();
  if (header.get_header().compare(current_header_str) != 0) {
    this->out << header.get_string_id() << std::endl;
    this->out << header.get_header() << std::endl;
    this->current_header = header;
  }
}
void tuddbs::dbsCSVFileWriter::append(tuddbs::dbsSerializable const & serializable) {
  auto const serialized = serializable.serialize();
  current_line_field_count += serialized.size();
  for (auto const & [key, value] : serialized) {
    if (this->current_line.empty()) {
      this->current_line += value;
    } else {
      this->current_line += separator + value;
    }
  }
  if (this->current_line_field_count == current_header.get_header_field_count()) {
    this->out << current_line << std::endl;
    this->current_line = "";
    this->current_line_field_count = 0;
  } else if (current_line_field_count > current_header.get_header_field_count()) {
    std::cerr << "Field count mismatch. Expected " << current_header.get_header_field_count() << " fields, but got " << current_line_field_count << " fields." << std::endl;
    std::cerr << "Header: " << current_header.get_header() << std::endl;
    std::cerr << "Current Data: " << current_line << std::endl;
    exit(-21);
  }
}