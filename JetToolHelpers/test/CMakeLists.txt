if(USE_ATHENA)
    add_compile_definitions(USE_ATHENA=yes)
endif()

add_executable(myTest "./R4ComponentsTest.cpp")
add_executable(JetContextUnitTest "./JetContextUnitTest.cpp")
add_executable(InputVariableUnitTest "./InputVariableUnitTest.cpp")

# is available because of compilation order
target_link_libraries(myTest JetToolHelpersLib)
target_include_directories(myTest PUBLIC ".")

target_link_libraries(JetContextUnitTest JetToolHelpersLib)
target_include_directories(JetContextUnitTest PUBLIC ".")

target_link_libraries(InputVariableUnitTest JetToolHelpersLib)
target_include_directories(InputVariableUnitTest PUBLIC ".")

# copy file to build/test directory.
configure_file(R4_AllComponents.root ${CMAKE_CURRENT_BINARY_DIR}/R4_AllComponents.root COPYONLY)
add_test(firstTest myTest)
add_test(JetContextUnitTest JetContextUnitTest)
add_test(InputVariableUnitTest InputVariableUnitTest)