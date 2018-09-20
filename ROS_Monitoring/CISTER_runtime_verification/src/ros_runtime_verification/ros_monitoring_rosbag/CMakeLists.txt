include_directories(${catkin_INCLUDE_DIRS})
add_executable(ros_monitoring_rosbag ros_monitoring_rosbag.cpp)
target_link_libraries(ros_monitoring_rosbag ${catkin_LIBRARIES})
install(TARGETS ros_monitoring_rosbag
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
