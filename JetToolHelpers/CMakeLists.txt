################################################################################
# Package: SSS
################################################################################

# Declare the package name:
atlas_subdir( JetToolHelpers )

# Extra dependencies, based on the environment:
set( EXTRA_DEPS )
if( XAOD_STANDALONE )
   set( EXTRA_DEPS Control/xAODRootAccess )
else()
   set( EXTRA_DEPS GaudiKernel )
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
   ${EXTRA_DEPS} )

# External dependencies:
find_package( ROOT COMPONENTS Core Tree MathCore Hist RIO Graf Gpad )

# Libraries in the package:
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

# Executable(s) in the package:
if( XAOD_STANDALONE )
   atlas_add_executable( JetToolHelpers_test_xAOD
      util/test_xAOD.cpp
      INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
      LINK_LIBRARIES ${ROOT_LIBRARIES} AsgTools xAODEventInfo xAODJet JetToolHelpersLib
      xAODTracking xAODCore xAODRootAccess )
   
   atlas_add_executable( Histo1DTests
      test/Histo1DTests.cpp
      INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
      LINK_LIBRARIES ${ROOT_LIBRARIES} AsgTools xAODEventInfo xAODJet JetToolHelpersLib
      xAODTracking xAODCore xAODRootAccess )
   
   add_test(NAME tmpTest COMMAND Histo1DTests)
   
else()
   atlas_add_executable( JetToolHelpers_test_xAOD
      util/test_xAOD.cxx
      INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
      LINK_LIBRARIES ${ROOT_LIBRARIES} AsgTools xAODEventInfo xAODJet JetToolHelpersLib
      xAODTracking xAODCore POOLRootAccessLib xAODRootAccess )

endif()

