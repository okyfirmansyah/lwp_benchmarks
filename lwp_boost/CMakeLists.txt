cmake_minimum_required(VERSION 3.9)
project(lwp_boost)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost COMPONENTS context system thread fiber)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    add_executable(lwp_boost lwp_boost.cpp)
    target_link_libraries(lwp_boost ${Boost_LIBRARIES})
endif()



