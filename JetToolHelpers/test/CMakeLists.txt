add_executable(myTest "./firstTest.cpp")

# is available because of compilation order
target_link_libraries(myTest JetToolHelpersLib)

# copy file to build/test directory.
configure_file(R4_AllComponents.root ${CMAKE_CURRENT_BINARY_DIR}/R4_AllComponents.root COPYONLY)
add_test(firstTest myTest)