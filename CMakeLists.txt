cmake_minimum_required(VERSION 3.14)
project(crow_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
# includes
include_directories(crow/include)
include_directories(asio/asio/include)

# executable
add_executable(server main.cpp)


# idk anything that is going on here , chatgpt gave me this so i guess it means something
