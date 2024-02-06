add_subdirectory(Core)
add_subdirectory(Domain)
add_subdirectory(Application)
add_subdirectory(Infrastructure)
add_subdirectory(Presentation)

add_library(${LIBRARY_NAME} INTERFACE)

target_link_libraries(${LIBRARY_NAME}
INTERFACE
        CoreLib
        DomainLib
        ApplicationLib
        InfrastructureLib
        PresentationLib
        nlohmann_json::nlohmann_json
        fmt::fmt
        spdlog::spdlog
        cxxopts::cxxopts
        PkgConfig::Pistache
)

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        ${LIBRARY_NAME}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(${ENABLE_LTO})
    target_enable_lto(
        TARGET
        ${LIBRARY_NAME}
        ENABLE
        ON)
endif()

if(${ENABLE_CLANG_TIDY})
    add_clang_tidy_to_target(${LIBRARY_NAME})
endif()
