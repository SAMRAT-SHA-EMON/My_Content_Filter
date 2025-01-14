cmake_minimum_required(VERSION 3.10)

project(my_content_filter)

# Add the include directory
include_directories(include)

# Add the source files
add_executable(my_content_filter src/my_filter.cpp)

# Install the executable
install(TARGETS my_content_filter DESTINATION bin)

# Install the configuration files
install(DIRECTORY conf/ DESTINATION etc/my_content_filter)
