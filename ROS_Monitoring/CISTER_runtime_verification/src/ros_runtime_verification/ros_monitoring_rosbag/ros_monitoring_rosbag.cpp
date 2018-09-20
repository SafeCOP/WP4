// %Tag(FULLTEXT)%
#include "ros/ros.h"
#include "ros/console.h"
#include "std_msgs/String.h"
#include <ros_its_msgs/CAM_simplified.h>
#include <sstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cmath>
#include <string>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <boost/foreach.hpp>
#include <yaml-cpp/yaml.h>
#include <topic_tools/shape_shifter.h>
#include <ros_type_introspection/ros_introspection.hpp>
#define foreach BOOST_FOREACH

#define ARGC_MIN_REQUIRED 1 // minimum number of parameters required

//Global vars
using namespace std;
std::string car_name;
std::string car_name_leader;
uint32_t car_id;

ros::Publisher pub_RX_obu;

rosbag::Bag bag;


const int SLEEP_PERIOD = 1; // in seconds


/// \brief  Get ROS Sim time
/// \return  ROS Sim time
double getSimTime(){
	//ROS_INFO("Time: %f", ros::Time::now().toSec());
	return ros::Time::now().toSec();
}

/// \brief  Help fucntion
int help() {
	ROS_WARN("Missing [car_name]!");
	ROS_WARN("Usage: rosrun ros_mk5_integration ros_mk5_bridge [car_name]");
	return 1;
}

/// \brief  Car Status Information message generate function
/// \param  TODO To be implemented
void CAR_INFO_msg_received(const ros_its_msgs::CAM_simplified::ConstPtr& car_status){
  // ROS_INFO("Message Received!");
  // ROS_INFO("\tcar_name = %s", (car_status->car_name).c_str());
  // ROS_INFO("\tlatitude = %lf", car_status->latitude);
  // ROS_INFO("\tlongitude = %lf", car_status->longitude);
  // ROS_INFO("\taltitude_altitudeValue = %lf", car_status->altitude_altitudeValue);
  // ROS_INFO("\theading_headingValue = %lf", car_status->heading_headingValue);
  // ROS_INFO("\tspeed_speedValue = %lf", car_status->speed_speedValue);
  // ROS_INFO("\tdriveDirection = %d", car_status->driveDirection);
  // ROS_INFO("\tsteeringWheelAngle_steeringWheelAngleValue %lf", car_status->steeringWheelAngle_steeringWheelAngleValue);
  // ROS_INFO("\tgasPedalPercent_Value = %lf", car_status->gasPedalPercent_Value);
  // ROS_INFO("\tbrakePedalPercent_Value = %lf", car_status->brakePedalPercent_Value);

  bag.write("data", ros::Time::now(), car_status);

  std::vector<std::string> topics;
  topics.push_back(std::string("data"));

  rosbag::View view(bag, rosbag::TopicQuery(topics));
  int i = 0;
  foreach(rosbag::MessageInstance const m, view)
  {
    ros_its_msgs::CAM_simplified::ConstPtr s = m.instantiate<ros_its_msgs::CAM_simplified>();
    if (s != NULL){
      ROS_INFO("\tcar_name = %s", (s->car_name).c_str());
      ROS_INFO("\tlatitude = %lf", s->latitude);
      ROS_INFO("\tlongitude = %lf", s->longitude);
      ROS_INFO("\taltitude_altitudeValue = %lf", s->altitude_altitudeValue);
      i++;
    }
  }
  ROS_INFO("I = %d", i);

  return;
}

//void CAR_INFO_msg_received2(const ros::MessageEvent<ros_its_msgs::CAM_simplified const>& event){
//const std::string& publisher_name = event.getPublisherName();
//const ros::M_string& header = event.getConnectionHeader();
//ros::Time receipt_time = event.getReceiptTime();
//const ros_its_msgs::CAM_simplified::ConstPtr& msg = event.getMessage();
//ROS_INFO("\tpublisher_name = %s", publisher_name.c_str());
//ROS_INFO("\tcar_name = %s", (msg->car_name).c_str());

//using namespace RosIntrospection;

void topicCallback(const topic_tools::ShapeShifter::ConstPtr& msg,
 const std::string &topic_name,
 RosIntrospection::Parser& parser)
{

  const std::string&  datatype   =  msg->getDataType();
  const std::string&  definition =  msg->getMessageDefinition();

    // don't worry if you do this more than once: already registered message are not overwritten.
  parser.registerMessageDefinition( topic_name,
    RosIntrospection::ROSType(datatype),
    definition );

    // reuse these opbects to improve efficiency ("static" makes them persistent)
  static std::vector<uint8_t> buffer;
  static std::map<std::string,RosIntrospection::FlatMessage>   flat_containers;
  static std::map<std::string,RosIntrospection::RenamedValues> renamed_vectors;

  RosIntrospection::FlatMessage&   flat_container = flat_containers[topic_name];
  RosIntrospection::RenamedValues& renamed_values = renamed_vectors[topic_name];

    // copy raw memory into the buffer
  buffer.resize( msg->size() );
  ros::serialization::OStream stream(buffer.data(), buffer.size());
  msg->write(stream);


    // deserialize and rename the vectors
  parser.deserializeIntoFlatContainer( topic_name, absl::Span<uint8_t>(buffer), &flat_container, 100);
  parser.applyNameTransform( topic_name, flat_container, &renamed_values );

    // Print the content of the message
  printf("--------- %s ----------\n", topic_name.c_str() );
  for (auto it: renamed_values)
  {
    const std::string& key = it.first;
    const RosIntrospection::Variant& value   = it.second;
    printf(" %s = %f\n", key.c_str(), value.convert<double>() );
  }
  for (auto it: flat_container.name)
  {
    const std::string& key    = it.first.toStdString();
    const std::string& value  = it.second;
    printf(" %s = %s\n", key.c_str(), value.c_str() );
  }
}


struct CharacterType{
  std::string attribute1;
  std::string attribute2;
};

namespace YAML{
  template<>
  struct convert<CharacterType>{
    static bool decode(const Node& node, CharacterType& cType){ 
     cType.attribute1 = node["attribute1"].as<std::string>();
     cType.attribute2 = node["attribute2"].as<std::string>();
     return true;
   }
 };
}

/// \brief  Main
int main(int argc, char **argv){

	if (argc < 1 + ARGC_MIN_REQUIRED) {
		help();
		//ros::shutdown();
		return -1;
	}


	RosIntrospection::Parser parser;

	// Load file
	//YAML::Node lconf = YAML::LoadFile("data.yml");

	// how to read data
    //for(YAML::const_iterator it=lconf.begin();it!=lconf.end();++it) {
	    //std::string scalar;
	    //*it >> scalar;
	    //std::cout << "Found scalar: " << scalar << std::endl;
	//    std::string key = it->first.as<std::string>();
	    //std::string second = it->second.as<std::string>();
	//    std::cout << "Found key: " << key << std::endl;
	    //std::cout << "Found second: " << second << std::endl;
	//}

	std::set<std::string> topic_names;


	YAML::Node config = YAML::LoadFile("data.yml");
	const YAML::Node& topics = config["topics"];
	for (YAML::const_iterator it = topics.begin(); it != topics.end(); ++it) {
   const YAML::Node& subscriber = *it;
   std::cout << "name: " << subscriber["name"].as<std::string>() << "\n";
   std::cout << "topic: " << subscriber["topic"].as<std::string>() << "\n\n";
   topic_names.insert(subscriber["topic"].as<std::string>());
 }

	//YAML::Node characterType = node["subscribers"];
	//for(YAML::const_iterator it=characterType.begin();it != characterType.end();++it) {
	//   std::string key = it->first.as<std::string>();       // <- key
	//   cTypeList.push_back(it->second.as<CharacterType>()); // <- value
	//}


 bag.open("test.bag", rosbag::bagmode::Write | rosbag::bagmode::Read );

 std::stringstream node_name_ss;
 node_name_ss << "ROS_RT_MON_ROSBAG" << argv[1];
 ros::init(argc, argv, node_name_ss.str());

 car_name = argv[1];

 ros::NodeHandle n;

 std::stringstream rx_obu_topic;
 rx_obu_topic << "/" << argv[1] << "/RXNetwork";

 std::stringstream tx_obu_topic;
 tx_obu_topic << "/" << argv[1] << "/TXNetwork";

 std::stringstream car_info_topic;
 car_info_topic << "/" << argv[1] << "/carINFO";


	//Subscribe to TX car topic (not used yet)
	//ros::Subscriber pub_TX_obu   = n.advertise<ros_its_msgs::CAM_simplified>(tx_obu_topic.str(), 1, null);

	//Publish (CAM simplified messages) to RX car topic
	//pub_RX_obu	                   = n.advertise<ros_its_msgs::CAM_simplified>(rx_obu_topic.str(), 1);

	//Subscribe to car info topic and update OBU CAM message
	//ros::Subscriber sub_car_info	 = n.subscribe(car_info_topic.str(), 1, CAR_INFO_msg_received2);


    //boost::function<void(const topic_tools::ShapeShifter::ConstPtr&)> callback;
    //callback = [&parser, topic_name](const topic_tools::ShapeShifter::ConstPtr& msg) -> void
    //{
    //    topicCallback(msg, topic_name, parser) ;
    //};
    //subscribers.push_back( nh.subscribe(topic_name, 10, callback) );


 ros::master::V_TopicInfo advertized_topics;
 ros::master::getTopics(advertized_topics);

 for (const auto& topic_info: advertized_topics)
 {
  printf("advertized: %s\n", topic_info.name.c_str());
}

    // check that all the topics are being published
for (const std::string& topic_name: topic_names)
{
  bool found = false;
  for (const auto& topic_info: advertized_topics)
  {
    if( topic_info.name == topic_name)
    {
      found = true;
      break;
    }
  }
  if( !found )
  {
    printf("This topic has not been published yet: %s\n", topic_name.c_str());
  }
}

    // subscribers will be created dynamically, one for each element in topic_names
std::vector<ros::Subscriber> subscribers;


for (const std::string& topic_name: topic_names)
{
        //who is afraid of lambdas and boost::functions ?
  boost::function<void(const topic_tools::ShapeShifter::ConstPtr&) > callback;
  callback = [&parser, topic_name](const topic_tools::ShapeShifter::ConstPtr& msg) -> void
  {
    topicCallback(msg, topic_name, parser) ;
  };
  subscribers.push_back( n.subscribe(topic_name, 10, callback) );
}

    //callback = [&parser, car_info_topic.str()](const topic_tools::ShapeShifter::ConstPtr& msg) -> void { 
    //	topicCallback(msg, car_info_topic.str(), parser) ;
    //};

ros::spin();

bag.close();

return 0;
}
