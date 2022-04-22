add_executable(myTest "./firstTest.cpp")



# is available because of compilation order
target_link_libraries(myTest JetToolHelpersLib)
add_test(firstTest myTest WORKING_DIRECTORY "/home/gordon/Documents/gordon_bsci/Sem6/BachelorsProject/jettoolhelpers/test")