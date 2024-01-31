set(LIBRARY_SOURCES "")

set(LIBRARY_HEADERS 
    "${CMAKE_CURRENT_SOURCE_DIR}/Controllers/UserController.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Entities/User.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Interfaces/IUserRepository.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Repositories/UserRepository.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/UseCases/CreateUserUseCase.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/UseCases/GetUserUseCase.h"
)

set(LIBRARY_INCLUDES 
    "${CMAKE_CURRENT_SOURCE_DIR}"  
    "${CMAKE_CURRENT_SOURCE_DIR}/Controllers"  
    "${CMAKE_CURRENT_SOURCE_DIR}/Entities"  
    "${CMAKE_CURRENT_SOURCE_DIR}/Interfaces" 
    "${CMAKE_CURRENT_SOURCE_DIR}/Repositories" 
    "${CMAKE_CURRENT_SOURCE_DIR}/UseCases" 
    "${CMAKE_BINARY_DIR}/configured_files/include"  
)

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(${LIBRARY_NAME} PUBLIC ${LIBRARY_INCLUDES})
target_link_libraries(
    ${LIBRARY_NAME}
    PRIVATE nlohmann_json::nlohmann_json
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