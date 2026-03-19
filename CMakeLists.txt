cmake_minimum_required(VERSION 3.10)

project(UserManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# All cpp files
file(GLOB_RECURSE SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/utils/*.cpp"
)

# Create executable
add_executable(ums ${SRC_FILES})

# All headers
target_include_directories(ums PRIVATE include utils)

# Link to SQLite
target_link_libraries(ums sqlite3) 