#! /usr/bin/env python

#18070006007 - Final Project Question 1
#chmod u+x ~/catkin_ws/src/beginner_tutorials3/src/moveturtle.py
#launch file = moveTurtlespy.launch

import rospy
import sys
from geometry_msgs.msg import Twist
import random
import math
from std_srvs.srv import Empty
from turtlesim.srv import *

nodeid = str(sys.argv[1])
nodename = 'turtle'+nodeid


rospy.init_node("moveturtle", anonymous=True)

if nodename == "turtle1":
	color_r = rospy.get_param('r')
	color_g = rospy.get_param('g')
	color_b = rospy.get_param('b')

	rospy.wait_for_service("clear")

	rospy.set_param("background_r", color_r)
	rospy.set_param("background_g", color_g)
	rospy.set_param("background_b", color_b)

	clear_background = rospy.ServiceProxy('clear', Empty)
	clear_background()

velocity_publisher = rospy.Publisher(nodename+'/cmd_vel', Twist, queue_size=10)
vel_msg = Twist()

horizontal_distance = 4
vertical_distance = 0.75
speed = 1.0

def setPosition(coin):
	if coin: 
		return 0
	else:
		return (180 * math.pi) / 180

def tossCoin():
	rand = random.random()
	if rand <= 0.5:
		return True
	else:
		return False		


if nodename == "turtle1":
	spawn_turtle2 = rospy.ServiceProxy('spawn', Spawn)
	spawn_turtle2(5.544445, 9.4445, setPosition(tossCoin()), "turtle2")
	spawn_turtle3 = rospy.ServiceProxy('spawn', Spawn)
	spawn_turtle3(5.544445, 7.4445, setPosition(tossCoin()), "turtle3")
	spawn_turtle4 = rospy.ServiceProxy('spawn', Spawn)
	spawn_turtle4(5.544445, 3.4445, setPosition(tossCoin()), "turtle4")


def move(distance):
	vel_msg.linear.x = speed
	t0 = rospy.Time.now().to_sec()    
	current_distance = 0
	while(current_distance < distance):
		velocity_publisher.publish(vel_msg)
		t1 = rospy.Time.now().to_sec()
		current_distance = speed * (t1-t0)

	vel_msg.linear.x = 0
	velocity_publisher.publish(vel_msg)



def turn(turnspeed, angle):
	angular_speed = (turnspeed * math.pi) /180
	relative_angle = (angle * math.pi) / 180
	vel_msg.angular.z = abs(angular_speed) 
	t0 = rospy.Time.now().to_sec()    
	current_angle = 0

	while(current_angle < relative_angle):
		velocity_publisher.publish(vel_msg)
		t1 = rospy.Time.now().to_sec()
		current_angle = angular_speed * (t1-t0) 

	vel_msg.angular.z = 0
	velocity_publisher.publish(vel_msg)	




def turtleMovement():
	global horizontal_distance
	move(horizontal_distance)
	turn(15, 90)
	move(vertical_distance) 
	turn(15, 90)
	horizontal_distance = horizontal_distance*2.0
	move(horizontal_distance)
	turn(15, 90)
	move(vertical_distance) 
	turn(15, 90)
	horizontal_distance = horizontal_distance/2.0
	move(horizontal_distance)

	
if __name__ =='__main__':
    try:
	turtleMovement() 	                       
    	rospy.loginfo("Turtle stopped moving")            
    except rospy.ROSInterruptException: pass  	
		
