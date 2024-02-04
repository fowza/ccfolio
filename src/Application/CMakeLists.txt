# file(GLOB APPLICATION_SOURCES "Services/*.cpp" "Dtos/*.cpp" "Interfaces/*.cpp")
add_library(ApplicationLib INTERFACE)
target_include_directories(ApplicationLib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Dtos
    ${CMAKE_CURRENT_SOURCE_DIR}/Interfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/Services)
target_link_libraries(ApplicationLib INTERFACE
    DomainLib
    nlohmann_json::nlohmann_json
    fmt::fmt
    spdlog::spdlog
    cxxopts::cxxopts
    PkgConfig::Pistache)
