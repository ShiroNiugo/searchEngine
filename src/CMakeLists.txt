cmake_minimum_required(VERSION 3.8)

set(Src main.cpp converterJSON.cpp searchServer.cpp invertedIndex.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(searchEngine ${Src})

target_link_libraries(searchEngine nlohmann_json::nlohmann_json)
target_include_directories(searchEngine PUBLIC ${MY_INCLUDE_DIR})