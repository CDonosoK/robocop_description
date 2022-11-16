#!/usr/bin/env python

import serial, struct, rospy, tf
import numpy as np
from std_msgs.msg import String
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point, Pose, Quaternion, Twist, Vector3
from scipy.spatial.transform import Rotation

port = '/dev/ttyUSB1'
baud = 115200


def euler_to_quaternion(roll, pitch, yaw):
    rot = Rotation.from_euler('xyz', [yaw, pitch, roll], degrees=True)
    q = rot.as_quat()
    q = np.round(q, 3)
    return q[::-1]

def chatter_callback(mensaje):
    global x, y, th
    data_message = mensaje.data.split('|')
    x = float(data_message[0])
    y = float(data_message[1])
    th = float(data_message[2])

def odomNode():
    global x, y, th
    odom_broadcaster = tf.TransformBroadcaster()
    rospy.init_node('odomNode', anonymous=True)
    rospy.Subscriber("pose_topic", String, chatter_callback)
    

    freq = rospy.Rate(100)
    imu = serial.Serial(port, baud, timeout=1)
    current_time = rospy.Time.now()
    last_time = rospy.Time.now()

    x = 0.0
    y = 0.0
    th = 0.0


    while not rospy.is_shutdown():
        data = imu.read_until(b'U')
        current_time = rospy.Time.now()
        if data[0] == 0x61 and len(data) == 20:
            # Acceleration data
            acc = np.array(struct.unpack('<hhh', data[1:7]))/32768.0*16.0*9.8
            acc = np.round(acc, 3)

            # Angular velocity data
            gyro = np.array(struct.unpack('<hhh', data[7:13]))/32768.0*2000.0
            gyro = np.round(gyro, 3)
            
            # Angle data
            angle = np.array(struct.unpack('<hhh', data[13:19]))/32768.0*180.0
            angle = np.round(angle, 3)
            q = euler_to_quaternion(angle[0], angle[1], angle[2])

            print(angle[2])

            print(x, y, th)
            '''
            x += deltaX
            y += deltaY
            th += deltaTh
            '''

            
            odom_broadcaster.sendTransform(
                (x, y, th),
                q,
                current_time,
                "base_link",
                "odom"
            )

            last_time = current_time
            freq.sleep()
            


if __name__ == '__main__':
    odomNode()