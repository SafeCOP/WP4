cmake_minimum_required(VERSION 2.8.3)
project(ros_runtime_verification)

find_package(Boost REQUIRED COMPONENTS date_time thread)
find_package(catkin REQUIRED COMPONENTS message_generation rostime roscpp rosconsole rosbag roscpp_serialization ros_its_msgs ros_type_introspection ros_introspection topic_tools)

include_directories(${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
link_directories(${catkin_LIBRARY_DIRS} ${Boost_INCLUDE_DIRS})

catkin_package(CATKIN_DEPENDS message_runtime std_msgs)

macro(roscompile_macro T)
  add_executable(${T} ${T}/${T}.cpp)
  target_compile_options(${T} PUBLIC "-std=c++11")
  target_compile_options(${T} PUBLIC "-pthread")
  target_compile_options(${T} PUBLIC "-O0")
  target_compile_options(${T} PUBLIC "-fmessage-length=0")
  
  target_link_libraries(${T} ${catkin_LIBRARIES} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} yaml-cpp)
  install(TARGETS ${T}
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
endmacro()

foreach(dir
    ros_monitoring_rosbag
    ros_data_publish_simulator
    )
  roscompile_macro(${dir})
endforeach()

install(FILES
  launch/talker_listener.launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)
