#include <ros/ros.h>
#include <tf2_ros/static_transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <cstdio>
#include <tf2/LinearMath/Quaternion.h>

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>

#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include "std_msgs/String.h"
#include <string>
#include <sstream>

typedef pcl::PointXYZRGB PointT;
typedef pcl::PointCloud<PointT> PointCloud;
PointCloud::Ptr pointCloud(new PointCloud);
cv::Mat rgb, depth;

class SubscribeAndPublish
{
public:
  SubscribeAndPublish()
  {
    // "/published_topic" 토픽을 publish
    pub_= n_.advertise<PointCloud> ("points2",2);
    // "/subscribed_topic" 토픽을 subscribe
    image_sub_ = n_.subscribe("image", 10, &SubscribeAndPublish::imagecallback,this);
    depth_sub_ = n_.subscribe("depth_image", 10, &SubscribeAndPublish::depthcallback,this);
  }
 
  void imagecallback(const std_msgs::String::ConstPtr& msg)
  {
    std::string a;
    a="/home/pjh/Downloads/rgbd_dataset_freiburg2_desk/";
    a+=msg->data.c_str();
    rgb = cv::imread(a);

    // Zoom factor in camera
    double cx = 319.5;
    double cy = 239.5;
    double fx = 525.0;
    double fy = 525.0;
    double depthScale = 5000.0;

    // Define the format used by the point cloud: XYZRGB is used here

    // Calculate the XYZRGB value corresponding to each point
    pointCloud->header.frame_id = "camera";
    for ( int v=0; v<rgb.rows; v++ )
        for (int u=0; u<rgb.cols; u++)
        {
            unsigned int d = depth.ptr<unsigned short>(v)[u];
            if (d==0)
                continue;
            PointT p;
            p.z = double(d)/depthScale;
            p.x = (u-cx)*p.z/fx;
            p.y = (v-cy)*p.z/fy;
            p.b = rgb.data[v*rgb.step+u*rgb.channels()];
            p.g = rgb.data[v*rgb.step+u*rgb.channels()+1];
            p.r = rgb.data[v*rgb.step+u*rgb.channels()+2];
            pointCloud->points.push_back(p);
        }

    pcl_conversions::toPCL(ros::Time::now(), pointCloud->header.stamp);
    pub_.publish (pointCloud);
  }  
  void depthcallback(const std_msgs::String::ConstPtr& msg)
  {
    std::string a;
    a="/home/pjh/Downloads/rgbd_dataset_freiburg2_desk/";
    a+=msg->data.c_str();
    depth = cv::imread(a, -1);
  }  

private:    
  ros::NodeHandle n_;       // NodeHandle 선언
  ros::Publisher pub_;      // Publisher 선언
  ros::Subscriber image_sub_;     // Subscriber 선언
  ros::Subscriber depth_sub_;     // Subscriber 선언

}; 

int main(int argc, char **argv){
  ros::init(argc,argv, "pcl_tutorial");
  SubscribeAndPublish SAPObject;

  ros::spin();
  return 0;
};
