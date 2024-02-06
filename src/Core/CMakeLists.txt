add_library(CoreLib INTERFACE)
target_include_directories(CoreLib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Logging
    ${CMAKE_CURRENT_SOURCE_DIR}/Utility)
target_link_libraries(CoreLib INTERFACE
    nlohmann_json::nlohmann_json
    fmt::fmt
    spdlog::spdlog
    cxxopts::cxxopts
    PkgConfig::Pistache)
