#!/usr/bin/env python

import serial, struct, rospy
import numpy as np
from sensor_msgs.msg import Imu
from nav_msgs.msg import Odometry


connected = False
port = '/dev/ttyUSB0'
baud = 115200

def euler_to_quaternion(roll, pitch, yaw):
    cy = np.cos(yaw * 0.5)
    sy = np.sin(yaw * 0.5)
    cp = np.cos(pitch * 0.5)
    sp = np.sin(pitch * 0.5)
    cr = np.cos(roll * 0.5)
    sr = np.sin(roll * 0.5)

    q = np.zeros((4, 1))
    q[0] = cy * cp * cr + sy * sp * sr
    q[1] = cy * cp * sr - sy * sp * cr
    q[2] = sy * cp * sr + cy * sp * cr
    q[3] = sy * cp * cr - cy * sp * sr

    return q

def imuNode():
    imuPublisher = rospy.Publisher('/imu', Imu, queue_size=10)
    freq = rospy.Rate(100)
    imuMsg = Imu()
    imuMsg.header.frame_id = 'imu_link'
    imuMsg.orientation_covariance = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    imuMsg.angular_velocity_covariance = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    imuMsg.linear_acceleration_covariance = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

    imu = serial.Serial(port, baud, timeout=1)

    while not rospy.is_shutdown():
        data = imu.read_until(b'U')
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
            imuMsg.header.stamp = rospy.Time.now()
            imuMsg.linear_acceleration.x = acc[0]
            imuMsg.linear_acceleration.y = acc[1]
            imuMsg.linear_acceleration.z = acc[2]
            imuMsg.angular_velocity.x = gyro[0]
            imuMsg.angular_velocity.y = gyro[1]
            imuMsg.angular_velocity.z = gyro[2]
            imuMsg.orientation.x = q[0]
            imuMsg.orientation.y = q[1]
            imuMsg.orientation.z = q[2]
            imuMsg.orientation.w = q[3]
            imuPublisher.publish(imuMsg)
            freq.sleep()


if __name__ == '__main__':
    try:
        rospy.init_node('imuNode', anonymous=True)
        imuNode()
    except rospy.ROSInterruptException:
        pass