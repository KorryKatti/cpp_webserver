# cmake_minimum_required(VERSION 3.14)
# project(crow_server)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
# # includes
# include_directories(crow/include)
# include_directories(asio/asio/include)

# # executable
# add_executable(server main.cpp)


# # idk anything that is going on here , chatgpt gave me this so i guess it means something

# linux version

cmake_minimum_required(VERSION 3.14)
project(crow_server)

set(CMAKE_CXX_STANDARD 17)

# include paths
include_directories(crow/include)
include_directories(/usr/include) # asio + boost (from pacman)

# executable
add_executable(server main.cpp)

# thread lib required by Crow
target_link_libraries(server pthread)
