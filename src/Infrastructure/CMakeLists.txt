add_library(InfrastructureLib INTERFACE)
target_include_directories(InfrastructureLib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Database
    ${CMAKE_CURRENT_SOURCE_DIR}/Repositories
    ${ODB_INCLUDE_DIRS}
)
target_link_libraries(InfrastructureLib INTERFACE
DomainLib
nlohmann_json::nlohmann_json
${ODB_LIBRARIES}
fmt::fmt
spdlog::spdlog
cxxopts::cxxopts
PkgConfig::Pistache)
