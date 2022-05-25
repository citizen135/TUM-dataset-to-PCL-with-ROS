#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>

#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>

using namespace std;

typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;
int main( int argc, char** argv )
{

    ros::init(argc,argv,"pub_pcl");
    ros::NodeHandle nh;
    ros::Publisher pub= nh.advertise<PointCloud> ("points2",1);

    // data fetch
    cv::Mat rgb, depth;
    rgb = cv::imread("../data/rgb.png");
    depth = cv::imread("../data/depth.png", -1);

    // Zoom factor in camera 
    double cx = 319.5;
    double cy = 239.5;
    double fx = 525.0;
    double fy = 525.0;
    double depthScale = 5000.0;

    // Define the format used by the point cloud: XYZRGB is used here
    typedef pcl::PointXYZRGB PointT;
    typedef pcl::PointCloud<PointT> PointCloud;
    // Calculate the XYZRGB value corresponding to each point
    PointCloud::Ptr pointCloud(new PointCloud);
    pointCloud->header.frame_id = "my_world";
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
    ros::Rate loop_rate(4);
    while (nh.ok())
    {
      pcl_conversions::toPCL(ros::Time::now(), pointCloud->header.stamp);
      pub.publish (pointCloud);
      ros::spinOnce ();
      loop_rate.sleep ();
    }
    // Point cloud save
    pointCloud->is_dense = false;
    cout << "Common cloud point" << pointCloud->size() << "Point."<< endl;
    pcl::io::savePCDFileBinary("../data/cabinet.pcd", *pointCloud );
    return 0;
}

