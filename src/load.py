#!/usr/bin/env python3
import rospy
import pandas as pd
import time
import tf2_ros
import geometry_msgs.msg
import nav_msgs.msg
import sensor_msgs.msg
import std_msgs.msg
from cv_bridge import CvBridge

gt = pd.read_csv('/home/pjh/Downloads/rgbd_dataset_freiburg2_desk/groundtruth.txt',sep=" ")
depth = pd.read_csv('/home/pjh/Downloads/rgbd_dataset_freiburg2_desk/depth.txt',sep=" ")
rgb = pd.read_csv('/home/pjh/Downloads/rgbd_dataset_freiburg2_desk/rgb.txt',sep=" ")
pathh='/home/pjh/Downloads/rgbd_dataset_freiburg2_desk/'

for _ in [gt, depth, rgb]:
    _['timestamp'] = _['timestamp'].round(4)

merge=pd.merge(gt,depth,how='outer',on='timestamp')
merge=pd.merge(merge,rgb,how='outer',on='timestamp')
merge=merge.sort_values(by='timestamp')

camera_publisher = rospy.Publisher('/camera_pose', geometry_msgs.msg.PoseStamped, queue_size=2)
path_publisher = rospy.Publisher('/camera_path', nav_msgs.msg.Path, queue_size=2)
rgb_image_pub = rospy.Publisher("/image",std_msgs.msg.String, queue_size=2)
depth_image_pub = rospy.Publisher("/depth_image",std_msgs.msg.String, queue_size=2)


pa=nav_msgs.msg.Path()
ps=[]

br = tf2_ros.TransformBroadcaster()
t = geometry_msgs.msg.TransformStamped()
rg=sensor_msgs.msg.Image()
de=sensor_msgs.msg.Image()
rg_bridge = CvBridge()
de_bridge = CvBridge()


def camera_pose(msg):
    po=geometry_msgs.msg.PoseStamped()
    br = tf2_ros.TransformBroadcaster()
    t = geometry_msgs.msg.TransformStamped()
    
    t.header.stamp =rospy.Time.now()
    t.header.frame_id = "world"
    t.child_frame_id = "camera"
    if pd.isna(msg['tx']):
        pass
    else:
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
        path_publisher.publish(pa)
        camera_publisher.publish(po)
        br.sendTransform(t)

    if pd.isna(msg['depthfile']):
        pass
    else:
        depth_image_pub.publish(msg['depthfile'])
    if pd.isna(msg['rgbfile']):
        pass
    else:
        rgb_image_pub.publish(msg['rgbfile'])


if __name__ == '__main__':
    rospy.init_node('tf2_broadcaster')
    for i in range(1, len(merge)):
        record = merge.iloc[i]
        gap=merge.iloc[i]['timestamp']-merge.iloc[i-1]['timestamp']
        camera_pose(record)
        time.sleep(gap)

    rospy.spin()
