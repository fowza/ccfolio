set(DOMAIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Entities/odb/User-odb.cxx")

add_library(DomainLib ${DOMAIN_SOURCES})
target_include_directories(DomainLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Entities
    ${CMAKE_CURRENT_SOURCE_DIR}/Entities/odb
    ${CMAKE_CURRENT_SOURCE_DIR}/Interfaces
)

target_link_libraries(DomainLib
    PUBLIC
    nlohmann_json::nlohmann_json
    fmt::fmt
    spdlog::spdlog
    cxxopts::cxxopts
    PkgConfig::Pistache
)
