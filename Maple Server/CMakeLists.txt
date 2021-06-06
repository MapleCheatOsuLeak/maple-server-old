set(source_dir "${PROJECT_SOURCE_DIR}/src")

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.h)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
# todo: when this becomes too much, make this a loop
set(myDesiredRootForHeaderFileSearch "~/src")
file(GLOB my_include_directories ${myDesiredRootForHeaderFileSearch})
include_directories(${my_include_directories})

add_executable (mapleserver "Maple Server.cpp" "Maple Server.h")
add_executable (mapleserver "src/client.cpp" "src/include/client.h")
add_executable (mapleserver "src/tcp_client.cpp" "src/include/tcp_client.h")
add_executable (mapleserver "src/tcp_server.cpp" "src/include/tcp_server.h")
add_executable (mapleserver "" "src/include/server_observer.h")
add_executable (mapleserver "" "src/include/pipe_ret_t.h")
add_executable (mapleserver "" "src/include/client_observer.h")
add_executable (mapleserver "src/Packets/Heartbeat.cpp" "src/Packets/Heartbeat.h")
add_executable (mapleserver "src/Packets/Login.cpp" "src/Packets/Login.h")
add_executable (mapleserver "src/Utils/StringUtilities.cpp" "")

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL CONFIG REQUIRED)
target_link_libraries(mapleserver PRIVATE Threads::Threads)
target_link_libraries(mapleserver PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(mapleserver PRIVATE CURL::libcurl)