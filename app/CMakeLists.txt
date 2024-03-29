file(GLOB_RECURSE APP_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cc"
        ${CMAKE_CURRENT_SOURCE_DIR}/Controllers/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/Dtos/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/Entities/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/Interfaces/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/Repositories/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/Services/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/Middleware/*.cc

        # ODB generated files
        ${CMAKE_CURRENT_SOURCE_DIR}/Entities/odb/*.cxx
)

add_executable(${EXECUTABLE_NAME} ${APP_SOURCES})

target_include_directories(${EXECUTABLE_NAME} PUBLIC
        # Root
        ${CMAKE_CURRENT_SOURCE_DIR}/Controllers
        ${CMAKE_CURRENT_SOURCE_DIR}/Dtos
        ${CMAKE_CURRENT_SOURCE_DIR}/Entities
        ${CMAKE_CURRENT_SOURCE_DIR}/Interfaces
        ${CMAKE_CURRENT_SOURCE_DIR}/Repositories
        ${CMAKE_CURRENT_SOURCE_DIR}/Services
        ${CMAKE_CURRENT_SOURCE_DIR}/Middleware
        ${CMAKE_BINARY_DIR}/configured_files/include/

        # ODB generated files
        ${CMAKE_CURRENT_SOURCE_DIR}/Entities/odb

        # External
        ${Boost_INCLUDE_DIRS}
        ${ODB_INCLUDE_DIRS}
        ${ARGON2_INCLUDE_DIRS}
)

target_link_libraries(
    ${EXECUTABLE_NAME}
    PRIVATE ${LIBRARY_NAME}
            nlohmann_json::nlohmann_json
            fmt::fmt
            spdlog::spdlog
            cxxopts::cxxopts
        ${Boost_LIBRARIES}
        ${ODB_LIBRARIES}
        ${ARGON2_LIBRARIES}
        jwt-cpp)

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        ${EXECUTABLE_NAME}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(${ENABLE_LTO})
    target_enable_lto(
        TARGET
        ${EXECUTABLE_NAME}
        ENABLE
        ON)
endif()

if(${ENABLE_CLANG_TIDY})
    add_clang_tidy_to_target(${EXECUTABLE_NAME})
endif()
