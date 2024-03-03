# file(GLOB PRESENTATION_SOURCES "Controllers/*.cpp")
add_library(PresentationLib INTERFACE)
target_include_directories(PresentationLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Controllers ${CMAKE_CURRENT_SOURCE_DIR}/Middleware ${CMAKE_BINARY_DIR}/configured_files/include/)
target_link_libraries(PresentationLib INTERFACE
    ApplicationLib
    InfrastructureLib
    CoreLib
    nlohmann_json::nlohmann_json
    fmt::fmt
    spdlog::spdlog
    cxxopts::cxxopts
    PkgConfig::Pistache
    jwt-cpp)
