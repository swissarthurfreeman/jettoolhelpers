cmake_minimum_required(VERSION 3.0)

project(JetToolHelpers)

set(CMAKE_CXX_FLAGS "-pthread -std=c++17 -m64")
add_compile_options("-Wall")

option(IN_PRODUCTION "Set to off if mocking external includes" OFF)

if(NOT IN_PRODUCTION)
   set(SOURCES ./Root/InputVariable.cpp)
   set(HEADER_FILES
      ./JetToolHelpers/HistoInput.h
      ./JetToolHelpers/make_histo.h
      ./JetToolHelpers/InputVariable.h
      ./JetToolHelpers/JetContext.h
      ./JetToolHelpers/Mock.h)   # to mock root and athena-

   set(ROOT_DIR /home/gordon/Documents/gordon_bsci/Sem6/BProject/root)
   find_package( ROOT COMPONENTS Core Tree MathCore Hist RIO Graf Gpad)   # configs ROOT_INCLUDE_DIRS and ROOT_LIBRARIES

   #message(${ROOT_INCLUDE_DIRS})   # uncomment these lines to get the include dirs of your environment
   #message(${ROOT_LIBRARIES})      # very useful to setup include path in vscode ! 

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
else()
   add_compile_definitions(USING_XAOD)
endif()