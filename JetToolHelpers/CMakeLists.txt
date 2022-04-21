################################################################################
# Package: SSS
################################################################################
cmake_minimum_required(VERSION 3.16)

project(JetToolHelpers)

set(CMAKE_CXX_FLAGS "-pthread -std=c++17 -m64 -I/home/gordon/Root/install/include")

find_package( ROOT COMPONENTS Core Tree MathCore Hist RIO Graf Gpad )

message("${ROOT_INCLUDE_DIRS}")

set(SOURCES 
   "./Root/HistoInput.Ctr.cpp"
   "./Root/HistoInput.Static.cpp"
   "./Root/HistoInput.Tool.cpp"
   "./Root/InputVariable.cpp" 
   "./Root/JetContext.cpp"
   "./Root/Jet.cpp")

set(HEADER_FILES
   "./JetToolHelpers/HistoInput.h"
   "./JetToolHelpers/IInputBase.h"
   "./JetToolHelpers/InputVariable.h"
   "./JetToolHelpers/Jet.h"
   "./JetToolHelpers/JetContext.h")

add_library(JetToolHelpersLib ${HEADER_FILES} ${SOURCES})
target_include_directories(JetToolHelpersLib PUBLIC "." PUBLIC ${ROOT_INCLUDE_DIRS})
target_link_libraries( JetToolHelpersLib PUBLIC ${ROOT_LIBRARIES} )

add_executable(Refac "./util/test_refactor.cpp")
target_link_libraries(Refac JetToolHelpersLib)