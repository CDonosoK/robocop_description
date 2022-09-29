#!/usr/bin/env python

from sensor_msgs.msg import Joy
import sys, rospy
sys.path.append("/home/cdonosok/Escritorio/ROS/src/robocop_description/codes/encoders")
from geometry_msgs.msg import Twist

'''
ang izq x
ang izq y
ang der x
L2
R2
ang der y
----
cuadrado
equis
circulo
triangulo
L1
R1
L2
R2
Share
Options
L3
R3
exit
Medio

---
---

sudo ds4drv para conectar el control
'''

def chatter_callback(mensaje):
    joystick_izquierdo = (round(mensaje.axes[0], 3), round(mensaje.axes[1], 3)) #Eje X, Eje Y
    joystick_derecho = (round(mensaje.axes[2], 3), round(mensaje.axes[5], 3)) #Eje X, Eje Y
    botones = (mensaje.buttons[1], mensaje.buttons[2], mensaje.buttons[3], mensaje.buttons[0]) #Equis, Circulo, Triangulo, Cuadrado

    publisherMotores = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
    twist = Twist()
    twist.linear.x = joystick_izquierdo[1]
    twist.angular.z = joystick_derecho[0]
    publisherMotores.publish(twist)


def nodoPS4():
    nombreNodo = "nodo_ps4"
    idUnico = True
    rospy.init_node(nombreNodo, anonymous = idUnico)
 
    nombreTopico = "/j1/joy"
    tipoTopico = Joy 

    rospy.Subscriber(nombreTopico, tipoTopico, chatter_callback)
    rospy.spin()




if __name__ == "__main__":
    global arduino
    nodoPS4()
    
