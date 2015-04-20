cmake_minimum_required (VERSION 2.6)

project (Server)

set (Test_VERSION_MAJOR 0)
set (Test_VERSION_MINOR 0)

# std threads
find_package(Threads REQUIRED)

# ssl
find_package(OpenSSL REQUIRED)

# find boost libraries
set (Boost_USE_STATIC_LIBS ON)
set (Boost_MULTITHREADED ON)
set (Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS system date_time filesystem iostreams REQUIRED)

# additional includes
include_directories(${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

set(CPP_PATH "src")

file(GLOB SRC_FILES
	"${CPP_PATH}/*.cpp"
	"${CPP_PATH}/system/*.cpp"
)

add_definitions(-std=c++11)
add_executable (ServerMain ${SRC_FILES})
target_link_libraries (ServerMain ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES})

# copy config file
file( COPY "${PROJECT_SOURCE_DIR}/content/config.txt" DESTINATION "${PROJECT_BINARY_DIR}/")