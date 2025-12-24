cmake_minimum_required(VERSION 3.12)
project(OrderBook VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(OrderBook
    src/Orderbook.cpp 
    main.cpp 
)

set(SOURCES
    src/Orderbook.cpp
    main.cpp 
)
