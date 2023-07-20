cmake_minimum_required(VERSION 3.13)
project(tuddbs_benchmark)

set(release_cxx_flag "-O2")
set(warnings "-Wall;-Wextra;-Wpedantic")
set(release_warnings "-Winline")


add_library(tuddbs_benchmark STATIC)
set_target_properties(tuddbs_benchmark PROPERTIES CXX_STANDARD 20)
target_compile_options(tuddbs_benchmark PRIVATE ${release_cxx_flag} ${warnings} ${release_warnings})
target_include_directories(tuddbs_benchmark PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_sources(tuddbs_benchmark PUBLIC  ${PROJECT_SOURCE_DIR}/src/hardware/dbscpu.cpp 
                                        ${PROJECT_SOURCE_DIR}/src/measurement/dbsflushcache.cpp
                                        ${PROJECT_SOURCE_DIR}/src/filewriter/dbscsv_filewriter.cpp
                                        ${PROJECT_SOURCE_DIR}/src/dbsutils.cpp)



add_executable(tuddbs_benchmark_main ${PROJECT_SOURCE_DIR}/src/main.cpp)
set_target_properties(tuddbs_benchmark_main PROPERTIES CXX_STANDARD 20)
target_compile_options(tuddbs_benchmark_main PRIVATE ${release_cxx_flag} ${warnings} ${release_warnings})
target_include_directories(tuddbs_benchmark_main PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_sources(tuddbs_benchmark_main PUBLIC ${PROJECT_SOURCE_DIR}/src/hardware/dbscpu.cpp 
                                            ${PROJECT_SOURCE_DIR}/src/measurement/dbsflushcache.cpp
                                            ${PROJECT_SOURCE_DIR}/src/filewriter/dbscsv_filewriter.cpp
                                            ${PROJECT_SOURCE_DIR}/src/dbsutils.cpp)