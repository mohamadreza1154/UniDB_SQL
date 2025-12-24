cmake_minimum_required(VERSION 3.30)
project(UniDB_SQL)

set(CMAKE_CXX_STANDARD 20)

add_executable(UniDB_SQL
        src/main.cpp
        src/engine/ExecutionEngine.cpp
        src/engine/TransactionStack.cpp
        src/storage/ArrayCollection.cpp
        src/storage/LinkedListCollection.cpp
)
