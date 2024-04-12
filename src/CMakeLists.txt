add_library(${LIBRARY_NAME} STATIC)

file(GLOB_RECURSE SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Network/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/Utility/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/Security/*.cc
)

target_sources(${LIBRARY_NAME} PRIVATE ${SOURCES})

target_include_directories(${LIBRARY_NAME} PUBLIC
        # Root
        ${CMAKE_CURRENT_SOURCE_DIR}/Network
        ${CMAKE_CURRENT_SOURCE_DIR}/Utility
        ${CMAKE_CURRENT_SOURCE_DIR}/Security
        ${CMAKE_BINARY_DIR}/configured_files/include/

        # External
        ${Boost_INCLUDE_DIRS}
        ${ODB_INCLUDE_DIRS}
        ${ARGON2_INCLUDE_DIRS}
)

target_link_libraries(${LIBRARY_NAME}
        PUBLIC
        nlohmann_json::nlohmann_json
        fmt::fmt
        spdlog::spdlog
        cxxopts::cxxopts
        ${Boost_LIBRARIES}
        ${ODB_LIBRARIES}
        ${ARGON2_LIBRARIES}
        jwt-cpp
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
