#!/usr/bin/env python3
import rospy
import pandas as pd
import time
import tf2_ros
import geometry_msgs.msg
import nav_msgs.msg

groundtruth = pd.read_csv('/home/pjh/Downloads/rgbd_dataset_freiburg2_desk/groundtruth.txt',sep=" ")
depth = pd.read_csv('/home/pjh/Downloads/rgbd_dataset_freiburg2_desk/depth.txt',sep=" ")
rgb = pd.read_csv('/home/pjh/Downloads/rgbd_dataset_freiburg2_desk/rgb.txt',sep=" ")
camera_publisher = rospy.Publisher('/camera_pose', geometry_msgs.msg.PoseStamped, queue_size=2)
path_publisher = rospy.Publisher('/camera_path', nav_msgs.msg.Path, queue_size=2)
pa=nav_msgs.msg.Path()
ps=[]

def camera_pose(msg):
    po=geometry_msgs.msg.PoseStamped()
    br = tf2_ros.TransformBroadcaster()
    t = geometry_msgs.msg.TransformStamped()
    
    t.header.stamp =rospy.Time.now()
    t.header.frame_id = "world"
    t.child_frame_id = "camera"
    t.transform.translation.x = msg['tx']
    t.transform.translation.y = msg['ty']
    t.transform.translation.z = msg['tz']
    t.transform.rotation.x = msg['qx']
    t.transform.rotation.y = msg['qy']
    t.transform.rotation.z = msg['qz']
    t.transform.rotation.w = msg['qw']

    po.pose.position= t.transform.translation
    po.pose.orientation= t.transform.rotation
    po.header.stamp =rospy.Time.now()
    po.header.frame_id = "world"
    pa.header.stamp =rospy.Time.now()
    pa.header.frame_id = "world"
    ps.append(po)
    pa.poses=ps
    path_publisher.publish(pa)
    camera_publisher.publish(po)
    br.sendTransform(t)

if __name__ == '__main__':
    rospy.init_node('tf2_turtle_broadcaster')
    for i in range(4, len(groundtruth)):
        record = groundtruth.loc[i]
        camera_pose(record)
        time.sleep(0.01)

    rospy.spin()
