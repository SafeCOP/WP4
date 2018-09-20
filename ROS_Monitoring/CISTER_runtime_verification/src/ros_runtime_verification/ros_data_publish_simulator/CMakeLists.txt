include_directories(${catkin_INCLUDE_DIRS})
add_executable(ros_data_publish_simulator ros_data_publish_simulator.cpp)
target_link_libraries(ros_data_publish_simulator ${catkin_LIBRARIES})
install(TARGETS ros_data_publish_simulator
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
