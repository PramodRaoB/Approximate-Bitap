cmake_minimum_required(VERSION 3.22)
project(approximate_bitap)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -fopenmp-simd -fopenmp -mavx2 -DBUILD -DLARGE -g")
#set(CMAKE_CXX_FLAGS "-O3 -fopenmp-simd -fopenmp -mavx512bw -DBUILD -DLARGE -g")

if (BASE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBASE")
elseif (FLIP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFLIP")
elseif(DP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDP")
endif ()

add_executable(approximate_bitap driver.cpp baseline.cpp partition.cpp helper.cpp)
