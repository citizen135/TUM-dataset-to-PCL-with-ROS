#include <ros/ros.h>
#include <tf2_ros/static_transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <cstdio>
#include <tf2/LinearMath/Quaternion.h>

#include <fstream>


std::string static_turtle_name;

int main(int argc, char **argv)
{
  ros::init(argc,argv, "my_static_tf2_broadcaster");

  static tf2_ros::StaticTransformBroadcaster static_broadcaster;
  geometry_msgs::TransformStamped static_transformStamped;

  static_transformStamped.header.stamp = ros::Time::now();
  static_transformStamped.header.frame_id = "world";
  static_transformStamped.child_frame_id = "my_world";
  static_transformStamped.transform.translation.x = -0.1546;
  static_transformStamped.transform.translation.y = -1.4445;
  static_transformStamped.transform.translation.z = 1.4773;
  tf2::Quaternion quat;
  quat.setRPY(atof(argv[5]), atof(argv[6]), atof(argv[7]));
  static_transformStamped.transform.rotation.x = 0.6529;
  static_transformStamped.transform.rotation.y = -0.5483;
  static_transformStamped.transform.rotation.z = 0.3248;
  static_transformStamped.transform.rotation.w = -0.4095;

//static_transformStamped.transform.translation.x = atof(argv[2]);
  //static_transformStamped.transform.translation.y = atof(argv[3]);
  //static_transformStamped.transform.translation.z = atof(argv[4]);
  //tf2::Quaternion quat;
  //quat.setRPY(atof(argv[5]), atof(argv[6]), atof(argv[7]));
  //static_transformStamped.transform.rotation.x = quat.x();
  //static_transformStamped.transform.rotation.y = quat.y();
  //static_transformStamped.transform.rotation.z = quat.z();
  //static_transformStamped.transform.rotation.w = quat.w();
  static_broadcaster.sendTransform(static_transformStamped);
  ros::Rate loop_rate(4);
  while (1)
  {
    static_broadcaster.sendTransform(static_transformStamped);
    ros::spinOnce ();
    loop_rate.sleep ();
  }

  return 0;
};
