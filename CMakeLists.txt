# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
include_directories( . )

# Create a variable called helloworld_SOURCES containing all .cpp files:
set(webserver_SOURCES 
	lib/bitset.c 
	lib/tokenizer.c 
	lib/error.c 
	lib/general.c 
	lib/vector.c 
	lib/dstring.c 
	lib/line.c 
	lib/lines.c 
	lib/dynstringstream.c 
	lib/stringmap.c
	http/http_enums.c
	http/http_statusline.c
	http/http_requestline.c
	http/http.c 
#	cli/cli.c
	connections/connection.c
	connections/connections.c
	connections/responsefactory.c
	server/serverconfig.c 
	server/server.c 
	communication/requests.c 
	communication/response.c 
	webserver.c)

set(test_SOURCES 
	lib/general.c
	lib/bitset.c 
	lib/error.c 
	lib/vector.c 
	lib/dstring.c
	lib/tokenizer.c 
	tests.c )
 
# For a large number of source files you can create it in a simpler way
# using file() function:
# file(GLOB hellworld_SOURCES *.cpp)
 
# Create an executable file called helloworld from sources:
add_executable(webserver ${webserver_SOURCES})

# SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_REENTRANT -lpthread -luriparser")

#add_executable(wstest ${test_SOURCES})

