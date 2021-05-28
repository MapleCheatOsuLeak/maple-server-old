set(source_dir "${PROJECT_SOURCE_DIR}/src")

cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (CMakeTarget "Maple Server.cpp" "Maple Server.h")

# TODO: Add tests and install targets if needed.

set(CMAKE_CXX_STANDARD 17)