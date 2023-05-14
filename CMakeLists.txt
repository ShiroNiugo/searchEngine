cmake_minimum_required(VERSION 3.24)
project(searchEngine VERSION 0.1)

# Read the JSON file.
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/config.json MY_JSON_STRING)
string(JSON CUR_CONFIG GET ${MY_JSON_STRING} config)
string(JSON CUR_VERSION GET ${CUR_CONFIG} ${IDX} version)

if (${CUR_VERSION} EQUAL ${CMAKE_PROJECT_VERSION})
    set(CMAKE_CXX_STANDARD 17)

    add_executable(${PROJECT_NAME} "src/main.cpp" "header/converterJSON.h" "header/invertedIndex.h" "header/searchServer.h")

    include(FetchContent)

    #nlohmann_json
    FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
    FetchContent_MakeAvailable(json)

    target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

    #[===[]===]
    #GoogleTest
    FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)

    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

    set(gtest_disable_pthreads on)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    enable_testing()

    target_link_libraries(${PROJECT_NAME} PRIVATE gtest_main)

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME})
else ()
    message("config.json has incorrect file version")
endif ()