#!/usr/bin/env python

import rospy, tf
import numpy as np
from nav_msgs.msg import Odometry

def odomCallback(odom):
    # Broadcast transform
    br = tf.TransformBroadcaster()
    br.sendTransform(
        (odom.pose.pose.position.x, odom.pose.pose.position.y, 0),
        (odom.pose.pose.orientation.x, odom.pose.pose.orientation.y, odom.pose.pose.orientation.z, odom.pose.pose.orientation.w),
        rospy.Time.now(),
        "base_link",
        "odom")

def odomNode():
    rospy.init_node('odomNode', anonymous=True)
    rospy.Subscriber('odom', Odometry, odomCallback)
    rospy.spin()

if __name__ == '__main__':
    odomNode()