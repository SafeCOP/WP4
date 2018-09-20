// %Tag(FULLTEXT)%
#include "ros/ros.h"
#include "ros/console.h"
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include <ros_its_msgs/CAM_simplified.h>
#include <sstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cmath>
#include <string>
#include <iostream>
#include <cstdio>
#include <ctime>

#define ARGC_MIN_REQUIRED 1 // minimum number of parameters required

//Global vars
using namespace std;
std::string car_name;
std::string car_name_leader;
uint32_t car_id;

ros::Publisher pub_RX_obu;
ros::Publisher pub_carINFO;
ros::Publisher pub_car2INFO;

/// \brief  Get ROS Sim time
/// \return  ROS Sim time
double getSimTime(){
      //ROS_INFO("Time: %f", ros::Time::now().toSec());
      return ros::Time::now().toSec();
}

/// \brief  Help function
int help() {
  ROS_WARN("Missing [car_name]!");
  ROS_WARN("Usage: rosrun ros_mk5_integration ros_data_publish_simulator [car_name]");
  return 1;
}


/// \brief  Car Status Information message generate function
/// \param  TODO To be implemented
void carINFO_msg_generate(){
  
  //TODO: update msg_to_send values 
  ros_its_msgs::CAM_simplified msg_to_send;
  msg_to_send.car_name = "THIS IS A COMPLEX ROS MESSAGE";
  msg_to_send.latitude = rand() % 90 + 1;
  msg_to_send.longitude = rand() % 90 + 1;
  msg_to_send.altitude_altitudeValue = rand() % 10 + 1;
  msg_to_send.heading_headingValue = rand() % 3;
  msg_to_send.speed_speedValue = rand() % 100;
  msg_to_send.driveDirection = rand() % 3;
  msg_to_send.steeringWheelAngle_steeringWheelAngleValue = rand() % 1;
  msg_to_send.gasPedalPercent_Value = rand() % 1;
  msg_to_send.brakePedalPercent_Value = 0;


  pub_carINFO.publish(msg_to_send);
  ROS_INFO("Complex message generated!");
  return;
}

/// \brief  Message generator
/// \param  TODO To be implemented
void String_msg_generate(){
  
  //TODO: update msg_to_send values 
  std_msgs::String msg_to_send;
  std::stringstream ss;
  ss << "THIS IS A SIMPLE ROS MESSAGE";
  
  msg_to_send.data = ss.str();

  pub_car2INFO.publish(msg_to_send);
  ROS_INFO("Simple String message generated!");
  return;
}

/// \brief  Message generator
/// \param  TODO To be implemented
void Int_msg_generate(){
  
  //TODO: update msg_to_send values 
  std_msgs::Int16 msg_to_send;
  
  msg_to_send.data = 45;

  pub_car2INFO.publish(msg_to_send);
  ROS_INFO("Simple Int message generated!");
  return;
}
  
/// \brief  Main
int main(int argc, char **argv){

  if (argc < 1 + ARGC_MIN_REQUIRED) {
    help();
    ros::shutdown();
  }

  /* initialize random seed: */
  srand(time(NULL));

  std::stringstream node_name_ss;
  node_name_ss << "ROS_carINFO_gen_" << argv[1];
  ros::init(argc, argv, node_name_ss.str());
  
  car_name = argv[1];

  ros::NodeHandle n;

  std::stringstream rx_obu_topic;
  rx_obu_topic << "/" << argv[1] << "/RXNetwork";

  std::stringstream tx_obu_topic;
  tx_obu_topic << "/" << argv[1] << "/TXNetwork";
  
  std::stringstream car_info_topic;
  car_info_topic << "/" << argv[1] << "/carINFO";

  //Publish to car info topic and update OBU CAM message
  pub_carINFO   = n.advertise<ros_its_msgs::CAM_simplified>(car_info_topic.str(), 1);
  //Publish to car2 info topic and update OBU CAM message (TODO: correct this!!!!!!)
  pub_car2INFO   = n.advertise<std_msgs::Int16>("/car2/carINFO", 1);
  
  ros::Rate loop_rate(1);
  while (ros::ok())
  {
    
    
    ros::spinOnce();
    Int_msg_generate();
    carINFO_msg_generate();
    loop_rate.sleep();
  }

  return 0;
}
