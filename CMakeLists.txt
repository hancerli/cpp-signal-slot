cmake_minimum_required(VERSION 3.20)
project("sss")

# Set the C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)

# enable extra warnings
# add_compile_options(-Wall -Wextra -Wpedantic)

# add the main file
add_executable(main
    main.cpp
)
