# file(GLOB DOMAIN_SOURCES "Entities/*.cpp" "Interfaces/*.cpp")

add_library(DomainLib INTERFACE)
target_include_directories(DomainLib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Entities
    ${CMAKE_CURRENT_SOURCE_DIR}/Interfaces
    )

target_link_libraries(DomainLib
    INTERFACE
    nlohmann_json::nlohmann_json
    fmt::fmt
    spdlog::spdlog
    cxxopts::cxxopts
    PkgConfig::Pistache)
