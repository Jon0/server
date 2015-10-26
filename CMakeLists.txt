cmake_minimum_required (VERSION 2.6)

project (Server)

set(Test_VERSION_MAJOR 0)
set(Test_VERSION_MINOR 0)

# std threads
find_package(Threads REQUIRED)

# ssl
find_package(OpenSSL REQUIRED)

# additional includes
include_directories(${OPENSSL_INCLUDE_DIR})

#build server library
set(MAIN_LIBRARY_NAME "server")
add_subdirectory(src)
include_directories("${PROJECT_SOURCE_DIR}/src")

# build main executable
add_subdirectory(main)

#build test executable
add_subdirectory(test)

# write src path to binary dir
configure_file (
  "${PROJECT_SOURCE_DIR}/startup/src_path.in"
  "${PROJECT_BINARY_DIR}/src_path"
)

# copy update scripts
file(COPY "${PROJECT_SOURCE_DIR}/startup/git-update.py" DESTINATION "${PROJECT_BINARY_DIR}/")
file(COPY "${PROJECT_SOURCE_DIR}/startup/run.py" DESTINATION "${PROJECT_BINARY_DIR}/")

# copy config file
file(COPY "${PROJECT_SOURCE_DIR}/content/web_pw.conf" DESTINATION "${PROJECT_BINARY_DIR}/")
