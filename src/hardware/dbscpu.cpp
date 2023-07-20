#include "hardware/dbscpu.hpp"

void tuddbs::dbsCPU::determine_cpu_name() {
  std::regex cpu_regex("model name\\s*:\\s*(.*)");
  std::ifstream cpuinfo("/proc/cpuinfo");
  if (!cpuinfo.is_open()) {
    std::cerr << "Could not open /proc/cpuinfo for reading." << std::endl;
    exit(-50);
  }
  std::string line;
  std::smatch matches;

  while (std::getline(cpuinfo, line)) {
    if (std::regex_search(line, matches, cpu_regex)) {
      this->cpu_name = matches[1].str();
      return;
    }
  }
  this->cpu_name = "Unknown CPU";
}

void tuddbs::dbsCPU::determine_cache_sizes() {
  const std::string cachePath = "/sys/devices/system/cpu/cpu0/cache/";
  const std::string searchPrefix = "index";
  DIR* dir = opendir(cachePath.c_str());
  if (!dir) {
      std::cerr << "Failed to open the directory: " << cachePath << std::endl;
      exit(-50);
  }
  std::vector<std::string> cache_directories{};
  dirent* entry;
  while ((entry = readdir(dir))) {
      std::string filename = entry->d_name;
      if (filename.find(searchPrefix) == 0) {
          cache_directories.push_back(cachePath + filename);
      }
  }
  closedir(dir);

  this->cache_sizes.clear();
  this->cache_sizes.resize(cache_directories.size());
  for (size_t i = 0; i < cache_directories.size(); i++) {
    cache_sizes[i] = 0;
  }

  for (auto const & dirname : cache_directories) {
    std::ifstream level_file(dirname + "/level");
    std::ifstream size_file(dirname + "/size");
    if (level_file.is_open()) {
      if (size_file.is_open()) {
        std::string size_line;
        std::getline(size_file, size_line);
        // size_line is of format <size><unit>
        // unit is one of: k, m, g
        // extract the unit
        char unit = size_line.back();
        size_line.pop_back();
        // extract the size
        size_t size = std::stoi(size_line);
        switch (unit) {
          case 'k':
            size *= 1024;
            break;
          case 'K':
            size *= 1024;
            break;
          case 'm':
            size *= 1024*1024;
            break;
          case 'M':
            size *= 1024*1024;
            break;
          case 'g':
            size *= 1024*1024*1024;
            break;
          case 'G':
            size *= 1024*1024*1024;
            break;
          default:
            std::cerr << "Unknown unit \"" << unit << "\" for cache size." << std::endl;
            exit(-51);
        }
        std::string level_line;
        std::getline(level_file, level_line);
        cache_sizes[std::stoi(level_line)] += size;
        level_file.close();
        size_file.close();
      } else {
        level_file.close();
        std::cerr << "Could not open file \"" << dirname << "/size\" for reading the cache size." << std::endl;
        exit(-50); 
      }
    } else {
      std::cerr << "Could not open file \"" << dirname << "/level\" for reading the cache level." << std::endl;
      exit(-50); 
    }
  }
}

std::vector<std::pair<std::string, std::string>> 
tuddbs::dbsCPU::serialize() const {
  std::vector<std::pair<std::string, std::string>> result{};
  result.push_back(std::make_pair("cpu_name", cpu_name));
  for (size_t i = 0; i < cache_sizes.size(); i++) {
    if (cache_sizes[i] > 0) {
      result.push_back(std::make_pair("cache_size_L" + std::to_string(i) + " [B]", std::to_string(cache_sizes[i])));
    }
  }
  return result;
}