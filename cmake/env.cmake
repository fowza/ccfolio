cmake_minimum_required(VERSION 3.21)

set(dotenv "${CMAKE_CURRENT_SOURCE_DIR}/.env")

if (EXISTS "${dotenv}")
    file(STRINGS "${dotenv}" entries)
    foreach (entry IN LISTS entries)
        if (entry MATCHES "^([^=]+)=(.*)$")
            set(ENV{${CMAKE_MATCH_1}} "${CMAKE_MATCH_2}")
        else ()
            message(FATAL_ERROR "Malformed dotenv entry:\n${entry}")
        endif ()
    endforeach ()
endif ()
