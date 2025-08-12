cmake_minimum_required(VERSION 3.20)

# Set the project name
project(slacky)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

# Add executable
add_executable(slacky_test src/test/mvp.cpp)

# Link libraries
target_link_libraries(slacky_test 
    PRIVATE 
    CURL::libcurl 
    nlohmann_json::nlohmann_json
)

# Set output directory
set_target_properties(slacky_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)