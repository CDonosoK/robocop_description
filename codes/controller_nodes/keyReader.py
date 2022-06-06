#!/usr/bin/env python

import sys, select,tty, termios
import rospy
from std_msgs.msg import String

if __name__ == '__main__':
    key_pub = rospy.Publisher('keys', String, queue_size=1)
    rospy.init_node('key_publisher')
    rate = rospy.Rate(100)
    # Se debe alterar el comportamiento de la terminal para recibir las teclas inmediatamente
    old_attr = termios.tcgetattr(sys.stdin)
    tty.setcbreak(sys.stdin.fileno())
    print("For control your robot use:")
    print("     w")
    print("a         d")
    print("     s")
    print("Press any key to exit...")
    while not rospy.is_shutdown():
        if select.select([sys.stdin], [], [], 0)[0] == [sys.stdin]:
            key_pub.publish(sys.stdin.read(1))
        rate.sleep()
    # Se restaura el comportamiento de la terminal
    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_attr)