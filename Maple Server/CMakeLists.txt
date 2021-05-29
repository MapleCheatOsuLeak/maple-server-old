set(source_dir "${PROJECT_SOURCE_DIR}/src")

cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (mapleserver "Maple Server.cpp" "Maple Server.h")
add_executable (mapleserver "src/client.cpp" "src/include/client.h")
add_executable (mapleserver "src/tcp_client.cpp" "src/include/tcp_client.h")
add_executable (mapleserver "src/tcp_server.cpp" "src/include/tcp_server.h")
add_executable (mapleserver "" "src/include/server_observer.h")
add_executable (mapleserver "" "src/include/pipe_ret_t.h")
add_executable (mapleserver "" "src/include/client_observer.h")

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL CONFIG REQUIRED)
target_link_libraries(mapleserver PRIVATE Threads::Threads)
target_link_libraries(mapleserver PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(mapleserver PRIVATE CURL::libcurl)