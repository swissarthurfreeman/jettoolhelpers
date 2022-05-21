################################################################################
# Package: SSS
################################################################################

option(USE_ATHENA "Set to off if athena includes are being mocked" OFF)

# adds USE_ATHENA as compile definition accessible in source code.
# add_compile_definitions(USE_ATHENA=yes)

# cmake -D USE_ATHENA=yes
# External dependencies:
find_package( ROOT COMPONENTS Core Tree MathCore Hist RIO Graf Gpad )

# Declare the package name:
if( USE_ATHENA )
   message("ATTEMPTING TO BUILD")
   atlas_subdir( JetToolHelpers )

   set( extra_deps )
   if( XAOD_STANDALONE )
      set( extra_deps Control/xAODRootAccess )
   else()
      set( extra_deps GaudiKernel )
   endif()

   # Declare the package's dependencies:
   atlas_depends_on_subdirs(
      PUBLIC
      Control/AthToolSupport/AsgTools
      Event/xAOD/xAODEventInfo
      Event/xAOD/xAODJet
      Event/xAOD/xAODTracking
      PRIVATE
      PhysicsAnalysis/POOLRootAccess
      ${extra_deps} )


   # Create atlas library Libraries in the package:
   # and include all dependencies (root and atlas software stack)
   atlas_add_library( JetToolHelpersLib
      JetToolHelpers/*.h Root/*.cxx Root/*.cpp
      PUBLIC_HEADERS JetToolHelpers    
      INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
      LINK_LIBRARIES ${ROOT_LIBRARIES}
      AsgTools xAODEventInfo xAODJet xAODTracking
      PRIVATE_LINK_LIBRARIES xAODCore)

   if( NOT XAOD_STANDALONE )
      atlas_add_component( JetToolHelpers
         src/components/*.cxx
         LINK_LIBRARIES JetToolHelpersLib )
   endif()

   atlas_add_dictionary( JetToolHelpersDict
      JetToolHelpers/JetToolHelpersDict.h
      JetToolHelpers/selection.xml
      LINK_LIBRARIES JetToolHelpersLib )

   # Executable(s) in the package
   if( XAOD_STANDALONE )
      atlas_add_executable( JetToolHelpers_test_xAOD
         util/test_refactor.cpp
         INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
         LINK_LIBRARIES ${ROOT_LIBRARIES} AsgTools xAODEventInfo xAODJet JetToolHelpersLib
         xAODTracking xAODCore xAODRootAccess )
   else()
      atlas_add_executable( JetToolHelpers_test_xAOD
         util/test_refactor.cxx
         INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
         LINK_LIBRARIES ${ROOT_LIBRARIES} AsgTools xAODEventInfo xAODJet JetToolHelpersLib
         xAODTracking xAODCore POOLRootAccessLib xAODRootAccess )
   endif()

   message("Setup your IDE include dir paths with this : ")
   message(ROOT_INCLUDE_DIRS : ${ROOT_INCLUDE_DIRS})
   message(ATLAS_INCLUDE_DIRS : ${ROOT_INCLUDE_DIRS})
else()
   project(JetToolHelpers)

   set(CMAKE_CXX_FLAGS "-pthread -std=c++17 -m64 -I/home/gordon/Root/install/include")
   add_compile_options("-Wall")

   set(SOURCES 
      "./Root/HistoInput.Static.cpp"
      "./Root/HistoInput.Tool.cpp"
      "./Root/InputVariable.cpp" 
      "./Root/Jet.cpp")

   set(HEADER_FILES
      "./JetToolHelpers/HistoInput.h"
      "./JetToolHelpers/IInputBase.h"
      "./JetToolHelpers/InputVariable.h"
      "./JetToolHelpers/Jet.h"
      "./JetToolHelpers/JetContext.h")

   add_library(JetToolHelpersLib ${HEADER_FILES} ${SOURCES})
   target_include_directories(JetToolHelpersLib PUBLIC "." PUBLIC ${ROOT_INCLUDE_DIRS})
   target_link_libraries( JetToolHelpersLib PUBLIC ${PROJECT_BINARY_DIR} ${ROOT_LIBRARIES} )

   #enable_testing()
   #add_subdirectory(test)

   add_executable(perf_test "./perf_test.cpp")
   target_link_libraries(perf_test JetToolHelpersLib)
endif()


