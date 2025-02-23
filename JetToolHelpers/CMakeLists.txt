cmake_minimum_required(VERSION 3.0)

project(JetToolHelpers)

set(CMAKE_CXX_FLAGS "-pthread -std=c++17 -m64")
add_compile_options("-Wall")

set(SOURCES
./Root/HistoInput.Ctr.cpp
   ./Root/HistoInput.Static.cpp
   ./Root/HistoInput.Tool.cpp
   ./Root/InputVariable.cpp)

set(HEADER_FILES
   ./JetToolHelpers/HistoInput.h
   ./JetToolHelpers/IInputBase.h
   ./JetToolHelpers/InputVariable.h
   ./JetToolHelpers/JetContext.h
   ./JetToolHelpers/Mock.h)   # to mock root and athena-

set(ROOT_DIR /home/gordon/Documents/gordon_bsci/Sem6/BProject/root)
find_package( ROOT COMPONENTS Core Tree MathCore Hist RIO Graf Gpad)   # configs ROOT_INCLUDE_DIRS and ROOT_LIBRARIES

#message(${ROOT_INCLUDE_DIRS})
#message(${ROOT_LIBRARIES})

message("Adding JetToolHelpers library")
add_library(JetToolHelpers ${HEADER_FILES} ${SOURCES})

message("Adding include directories")
target_include_directories(JetToolHelpers PUBLIC "." PUBLIC ${ROOT_INCLUDE_DIRS})

message("Linking...")
target_link_libraries( JetToolHelpers PUBLIC ${PROJECT_BINARY_DIR} ${ROOT_LIBRARIES} )

enable_testing()
add_subdirectory(test)
message("Configuring Benchmarking...")
find_package(benchmark REQUIRED)

add_executable(perf_test "./perf_test.cpp")
target_link_libraries(perf_test JetToolHelpers benchmark::benchmark)
