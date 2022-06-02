cmake_minimum_required(VERSION 3.16)
project(Middleware1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(Middleware1 main.cpp Buffer.h Connection.h Sender.h Server.h Receiver.h Logging.cpp AbstractProxy.h AbstractSkeleton.h ServiceCommon.cpp ThreadPool.h)
