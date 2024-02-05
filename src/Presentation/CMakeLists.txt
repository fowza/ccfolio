# file(GLOB PRESENTATION_SOURCES "Controllers/*.cpp")
add_library(PresentationLib INTERFACE)
target_include_directories(PresentationLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Controllers)
target_link_libraries(PresentationLib INTERFACE
    ApplicationLib
    InfrastructureLib
    nlohmann_json::nlohmann_json
    fmt::fmt
    spdlog::spdlog
    cxxopts::cxxopts
    PkgConfig::Pistache)
