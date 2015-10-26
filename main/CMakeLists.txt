add_definitions(-std=c++14)

# main library
set(EXECUTABLE_NAME "run")

# server executable
file(GLOB MAIN_FILES
	"*.cpp"
)

add_executable(${EXECUTABLE_NAME} ${MAIN_FILES})

target_link_libraries(${EXECUTABLE_NAME} ${MAIN_LIBRARY_NAME} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES})
