add_definitions(-std=c++14)

# main library cpp files
file(GLOB SRC_FILES
	"*.cpp"
	"io/*.cpp"
	"linux/*.cpp"
	"server/*.cpp"
	"system/*.cpp"
	"type/*.cpp"
)

file(GLOB INCLUDE_FILES
	"*.cpp"
	"io/*.h"
	"linux/*.h"
	"server/*.h"
	"system/*.h"
	"type/*.h"
)

add_library(
	${MAIN_LIBRARY_NAME}
	SHARED
	${SRC_FILES}
	${INCLUDE_FILES}
)

target_link_libraries(
	${MAIN_LIBRARY_NAME}
	${CMAKE_THREAD_LIBS_INIT}
	${OPENSSL_LIBRARIES}
	${Boost_LIBRARIES}
)

