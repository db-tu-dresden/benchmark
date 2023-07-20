#ifndef TUDDBS_BENCHMARK_INCLUDE_FILEWRITER_dbsCSV_FILEWRITER_HPP
#define TUDDBS_BENCHMARK_INCLUDE_FILEWRITER_dbsCSV_FILEWRITER_HPP

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <tuple>

#include "../dbsutils.hpp"
#include "dbsserialize.hpp"

namespace tuddbs{

  template <typename... Args>
    std::string concat(std::string separator, Args&&... args) {
      std::ostringstream oss;
      bool first = true;
      ((oss << (first ? "" : separator) << args, first = false), ...);
      return oss.str();
    }
    
    class dbsCSVHeader {
      private:
        std::string separator;
        std::string header;
        size_t header_field_count;
        std::string string_id = "#!--Header -!#";
      public:
        ~dbsCSVHeader() = default;
        dbsCSVHeader(std::string separator)
        : separator(separator),
          header(""),
          header_field_count(0)
        {}
        template<typename... Args>
          void append_bulk(Args&&... args) {
            header_field_count += sizeof...(Args);
            header += separator + concat(separator, std::forward<Args>(args)...);
          }
        void append(dbsSerializable const & serializable);
        std::string get_header() const {
          return header;
        }
        size_t get_header_field_count() const {
          return header_field_count;
        }
        std::string get_string_id() const {
          return string_id;
        }
    };


    class dbsCSVFileWriter {
      private:
        std::string filename;
        std::ofstream out;
        std::string separator;
        dbsCSVHeader current_header;
        std::string current_line = "";
        size_t current_line_field_count = 0;
      private:
        std::string get_latest_header() const;
      public:
        dbsCSVFileWriter(std::string base_path, std::string separator)
        : filename(base_path + "_" + now_format_str() + ".csv"),
          out(filename, std::ios::out | std::ios::app),
          separator(separator),
          current_header(separator)
        {
          if (!out.is_open()) {
            std::cerr << "Could not open file \"" << base_path << "_" << now_format_str() << ".csv\" for writing." << std::endl;
            exit(-20);
          }
        }

        virtual ~dbsCSVFileWriter() {
          out.close();
        }
      
      public:
        void set_field_names(dbsCSVHeader header);
        auto create_field_names_description() {
          return dbsCSVHeader(separator);
        }
        template<typename... Args>
          void create_and_set_field_names_description(Args&&... args) {
            dbsCSVHeader tmp_header{separator};
            tmp_header.append(std::forward<Args>(args)...);
            set_field_names(tmp_header);
          }
        
        
        template<typename... Args>
          void write_line(Args&&... args) {
            if (current_header.get_header_field_count() != sizeof...(Args)) {
              std::cerr << "Field count mismatch. Expected " << current_header.get_header_field_count() << " fields, but got " << sizeof...(Args) << " fields." << std::endl;
              std::cerr << "Header: " << current_header.get_header() << std::endl;
              std::cerr << "Data: " << concat(separator, std::forward<Args>(args)...) << std::endl;
              exit(-21);
            }
            out << concat(separator, std::forward<Args>(args)...) << std::endl;
          }

        void append(dbsSerializable const & serializable);
        template<typename... Args>
          void append_bulk(Args&&... args) {
            current_line_field_count += sizeof...(Args);
            if (current_line.empty()) {
              current_line += concat(separator, std::forward<Args>(args)...);
            } else {
              current_line += separator + concat(separator, std::forward<Args>(args)...);
            }
            if (current_line_field_count == current_header.get_header_field_count()) {
              out << current_line << std::endl;
              current_line = "";
              current_line_field_count = 0;
            } else if (current_line_field_count > current_header.get_header_field_count()) {
              std::cerr << "Field count mismatch. Expected " << current_header.get_header_field_count() << " fields, but got " << current_line_field_count << " fields." << std::endl;
              std::cerr << "Header: " << current_header.get_header() << std::endl;
              std::cerr << "Current Data: " << current_line << std::endl;
              exit(-21);
            }
          }
    };


  using csv_filewriter_t = dbsCSVFileWriter;
}

#endif //TUDDBS_BENCHMARK_INCLUDE_FILEWRITER_dbsCSV_FILEWRITER_HPP