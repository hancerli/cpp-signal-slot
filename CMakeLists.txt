cmake_minimum_required(VERSION 3.20)
project("design-patterns")


# Set the C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)

# enable extra warnings
# add_compile_options(-Wall -Wextra -Wpedantic)

# add compiler flags to see macro expansions
add_compile_options(-dD)
# add_compile_options(-E)


# add the main file
add_executable(main
    main.cpp
    signal_slot.h
)
