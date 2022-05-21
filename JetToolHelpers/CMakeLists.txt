cmake_minimum_required(VERSION 3.22.1)

project(JetToolHelpers)

set(CMAKE_CXX_FLAGS "-pthread -std=c++17 -m64")
add_compile_options("-Wall")

set(SOURCES 
   ./Root/HistoInput.Static.cpp
   ./Root/HistoInput.Tool.cpp
   ./Root/InputVariable.cpp)

set(HEADER_FILES
   ./JetToolHelpers/HistoInput.h
   ./JetToolHelpers/IInputBase.h
   ./JetToolHelpers/InputVariable.h
   ./JetToolHelpers/JetContext.h
   ./JetToolHelpers/Mock.h)   # to mock root and athena-

add_library(JetToolHelpers ${HEADER_FILES} ${SOURCES})
target_include_directories(JetToolHelpers PUBLIC "." PUBLIC ${ROOT_INCLUDE_DIRS})
target_link_libraries( JetToolHelpers PUBLIC ${PROJECT_BINARY_DIR} ${ROOT_LIBRARIES} )

#enable_testing()
#add_subdirectory(test)

add_executable(perf_test "./perf_test.cpp")
target_link_libraries(perf_test JetToolHelpers)