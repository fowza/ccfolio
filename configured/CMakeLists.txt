set(SECRET_KEY $ENV{SECRET_KEY})
set(MYSQL_USER $ENV{MYSQL_USER})
set(MYSQL_PASSWORD $ENV{MYSQL_PASSWORD})
set(MYSQL_DATABASE $ENV{MYSQL_DATABASE})
set(MYSQL_HOST $ENV{MYSQL_HOST})

configure_file(
        "config.hpp.in" "${CMAKE_BINARY_DIR}/configured_files/include/config.hpp"
        @ONLY
        NEWLINE_STYLE UNIX
)
