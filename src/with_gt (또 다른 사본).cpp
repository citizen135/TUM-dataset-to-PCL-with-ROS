#include <ros/ros.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/transform_listener.h>
#include <tf2/convert.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Path.h>
#include <cstdio>
#include <tf2/LinearMath/Quaternion.h>

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <pcl/common/transforms.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <istream>

#include <numeric>
#include <cmath>

#include <pcl/filters/voxel_grid.h>

using namespace std;
string data_path="/home/pjh/data/rgbd_dataset_freiburg2_desk/";
string data_fill=data_path+"fill.csv";

//csv 파일안에 있는 데이터를 읽어서 vector에 저장한다.
vector<string> csv_read_row(istream &file, char delimiter);
cv::Mat rgb, depth;

int main(int argc, char **argv){
  // Zoom factor in camera
  double cx = 319.5;
  double cy = 239.5;
  double fx = 525.0;
  double fy = 525.0;
  double depthScale = 5000.0;
  ros::init(argc,argv, "pcl_tutorial");
  ros::NodeHandle nh;       // NodeHandle 선언
  ros::Publisher pub_= nh.advertise<pcl::PointCloud<pcl::PointXYZRGB>> ("points2",2);      // Publisher 선언
  ros::Publisher pub2= nh.advertise<pcl::PointCloud<pcl::PointXYZRGB>> ("points1",2);      // Publisher 선언
  ros::Publisher pub3= nh.advertise<pcl::PointCloud<pcl::PointXYZRGB>> ("points3",2);      // Publisher 선언
  ros::Publisher pub1= nh.advertise<nav_msgs::Path> ("camera_path",2);
  nav_msgs::Path path;


  ifstream file(data_fill); 
  vector<string> b_row=csv_read_row(file, ',');
  b_row=csv_read_row(file, ',');
  double gap;

  // tf2::TransformListener listener;
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr mapCloud(new pcl::PointCloud<pcl::PointXYZRGB>);
  pcl::VoxelGrid<pcl::PointXYZRGB> sor;
//   pcl::PointCloud<pcl::PointXYZRGB> pointCloud;
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr pc_transformed(new pcl::PointCloud<pcl::PointXYZRGB>);
  mapCloud->header.frame_id = "world";
  while(file.good()) //eof, bad, fail 함수가 거짓의 참을 반환할 때까지..
    {
    vector<string> row = csv_read_row(file, ',');
    float gap;
    static tf2_ros::TransformBroadcaster br;
    geometry_msgs::TransformStamped transformStamped;
    geometry_msgs::PoseStamped pose;

    pose.header.stamp= ros::Time::now();
    pose.header.frame_id ="world";
    pose.pose.position.x=stod(row[2]);
    pose.pose.position.y=stod(row[3]);
    pose.pose.position.z=stod(row[4]);
    path.header.stamp = ros::Time::now();
    path.header.frame_id ="world";
    path.poses.push_back(pose);

    transformStamped.header.stamp = ros::Time::now();
    transformStamped.header.frame_id = "world";
    transformStamped.child_frame_id = "camera";
    transformStamped.transform.translation.x = stod(row[2]);
    transformStamped.transform.translation.y = stod(row[3]);
    transformStamped.transform.translation.z = stod(row[4]);
    transformStamped.transform.rotation.x = stod(row[5]);
    transformStamped.transform.rotation.y = stod(row[6]);
    transformStamped.transform.rotation.z = stod(row[7]);
    transformStamped.transform.rotation.w = stod(row[8]);   //여기까지 tf

    string depth_file;
    depth_file=data_path+row[9];
    depth = cv::imread(depth_file, -1);
    string rgb_file;
    rgb_file=data_path+row[10];
    rgb = cv::imread(rgb_file);

    pcl::PointCloud<pcl::PointXYZRGB> pointCloud;
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr pc_transformed(new pcl::PointCloud<pcl::PointXYZRGB>);
    pointCloud.header.frame_id = "world";
    for ( int v=0; v<rgb.rows; v++ )
        for (int u=0; u<rgb.cols; u++)
        {
            unsigned int d = depth.ptr<unsigned short>(v)[u];
            if (d==0)
                continue;
            pcl::PointXYZRGB p;
            p.z = (double(d)/depthScale);
            p.x = ((u-cx)*p.z/fx);
            p.y = ((v-cy)*p.z/fy);
            
            p.b = rgb.data[v*rgb.step+u*rgb.channels()];
            p.g = rgb.data[v*rgb.step+u*rgb.channels()+1];
            p.r = rgb.data[v*rgb.step+u*rgb.channels()+2];
            
            pointCloud.points.push_back(p);
        }
    vector<float> quato { stod(row[5]), stod(row[6]), stod(row[7]), stod(row[8])};
    double inner;
    inner=inner_product(quato.begin(), quato.end(), quato.begin(), 0.0);
    vector<double> quato1={quato[0]*sqrt(2.0/inner),quato[1]*sqrt(2.0/inner),quato[2]*sqrt(2.0/inner),quato[3]*sqrt(2.0/inner)};

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr ptr_transformed(new pcl::PointCloud<pcl::PointXYZRGB>);
    Eigen::Matrix4f trans;
    trans<< 1.0-quato1[1]*quato1[1]-quato1[2]*quato1[2],   quato1[0]*quato1[1]-quato1[2]*quato1[3],  quato1[0]*quato1[2]+quato1[1]*quato1[3], stod(row[2]),
            quato1[0]*quato1[1]+quato1[2]*quato1[3],   1.0-quato1[0]*quato1[0]-quato1[2]*quato1[2],  quato1[1]*quato1[2]-quato1[0]*quato1[3], stod(row[3]),
            quato1[0]*quato1[2]-quato1[1]*quato1[3],   quato1[1]*quato1[2]+quato1[0]*quato1[3],  1.0-quato1[0]*quato1[0]-quato1[1]*quato1[1], stod(row[4]),
            0.0,   0.0,  0.0,     1.0;
    // cout<<pointCloud<<endl;
    // cout<<*ptr_transformed<<endl;
    pcl::transformPointCloud(pointCloud, *ptr_transformed, trans);
    pc_transformed=ptr_transformed;
    pc_transformed->header.frame_id = "world";
    pcl_conversions::toPCL(ros::Time::now(), pointCloud.header.stamp);
    pcl_conversions::toPCL(ros::Time::now(), pc_transformed->header.stamp);
    pcl_conversions::toPCL(ros::Time::now(), mapCloud->header.stamp);
    *mapCloud+=*pc_transformed;
    sor.setInputCloud (mapCloud);
    sor.setLeafSize (0.01f, 0.01f, 0.01f);
    sor.filter (*mapCloud);
    

    // cout<<pc_transformed.points[0]<<endl;
    
    // pcl_conversions::toPCL(ros::Time::now(), mapCloud.header.stamp);
    // pub2.publish (pointCloud);
    pub3.publish (*mapCloud);
    // pub_.publish (pc_transformed);
    // pub2.publish (mapCloud);
    pub1.publish(path);
    br.sendTransform(transformStamped);
    gap=stod(row[1])-stod(b_row[1]);       
    ros::Duration(gap).sleep();
    b_row=row;
    }

  return 0;
};

vector<string> csv_read_row(istream &file, char delimiter)
{
    stringstream ss;
    bool inquotes = false;
    vector<string> row;//relying on RVO
 
    while(file.good())
    {
        char c = file.get();
        if (!inquotes && c=='"') 
        {
            inquotes=true;
        }
        else if (inquotes && c=='"') 
        {
            if ( file.peek() == '"')
            {
                ss << (char)file.get();
            }
            else 
            {
                inquotes=false;
            }
        }
        else if (!inquotes && c==delimiter) 
        {
            row.push_back( ss.str() );
            ss.str("");
        }
        else if (!inquotes && (c=='\r' || c=='\n') )
        {
            if(file.peek()=='\n') { file.get(); }
            row.push_back( ss.str() );
            return row;
        }
        else
        {
            ss << c;
        }
    }
}
