# file(GLOB INFRASTRUCTURE_SOURCES "Database/*.cpp" "Repositories/*.cpp")
add_library(InfrastructureLib INTERFACE)
target_include_directories(InfrastructureLib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Database
    ${CMAKE_CURRENT_SOURCE_DIR}/Repositories
)
target_link_libraries(InfrastructureLib INTERFACE
DomainLib
nlohmann_json::nlohmann_json
fmt::fmt
spdlog::spdlog
cxxopts::cxxopts
PkgConfig::Pistache)
