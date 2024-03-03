# file(GLOB APPLICATION_SOURCES "Services/*.cpp" "Dtos/*.cpp" "Interfaces/*.cpp")
add_library(ApplicationLib INTERFACE)
target_include_directories(ApplicationLib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Dtos
    ${CMAKE_CURRENT_SOURCE_DIR}/Interfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/Services
    ${ARGON2_INCLUDE_DIRS})
target_link_libraries(ApplicationLib INTERFACE
    DomainLib
    CoreLib
    nlohmann_json::nlohmann_json
    fmt::fmt
    spdlog::spdlog
    cxxopts::cxxopts
    PkgConfig::Pistache
    jwt-cpp
    ${ARGON2_LIBRARIES})
