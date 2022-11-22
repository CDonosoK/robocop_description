#!/usr/bin/env python

import serial, struct, rospy
import numpy as np
from sensor_msgs.msg import Imu
from scipy.spatial.transform import Rotation

port = '/dev/ttyUSB0'
baud = 115200

def euler_to_quaternion(roll, pitch, yaw):
    rot = Rotation.from_euler('xyz', [yaw, pitch, roll], degrees=True)
    q = rot.as_quat()
    q = np.round(q, 3)
    return q[::-1]


def imuNode():
    global x, y, th
    rospy.init_node('imuNode', anonymous=True)
    imuPublisher = rospy.Publisher('imu', Imu, queue_size=10)
    

    freq = rospy.Rate(100)
    imu = serial.Serial(port, baud, timeout=1)

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

            # Publish IMU data
            imu_msg = Imu()
            imu_msg.header.stamp = current_time
            imu_msg.header.frame_id = 'base_link'
            imu_msg.orientation.x = q[0]
            imu_msg.orientation.y = q[1]
            imu_msg.orientation.z = q[2]
            imu_msg.orientation.w = q[3]
            imu_msg.angular_velocity.x = gyro[0]
            imu_msg.angular_velocity.y = gyro[1]
            imu_msg.angular_velocity.z = gyro[2]
            imu_msg.linear_acceleration.x = acc[0]
            imu_msg.linear_acceleration.y = acc[1]
            imu_msg.linear_acceleration.z = acc[2]

            imuPublisher.publish(imu_msg)


            freq.sleep()
            


if __name__ == '__main__':
    imuNode()