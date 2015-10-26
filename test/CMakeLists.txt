add_definitions(-std=c++14)

# main library
set(TEST_EXECUTABLE_NAME "runtests")

# server executable
file(GLOB TEST_FILES
	"*.cpp"
)

add_executable(${TEST_EXECUTABLE_NAME} ${TEST_FILES})

target_link_libraries(${TEST_EXECUTABLE_NAME} ${MAIN_LIBRARY_NAME} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES})
