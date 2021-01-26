//18070006007 Final Project -Q1

#include <ros/ros.h>
#include <iostream>
#include <turtlesim/Spawn.h>
#include <time.h>
#include <iostream>
#include <geometry_msgs/Twist.h>
#include <std_srvs/Empty.h>
#include <cmath>
#include <string>

ros::Publisher vel_pub;
geometry_msgs::Twist vel_msg;
ros::Publisher vel_pub2;
geometry_msgs::Twist vel_msg2;
ros::Publisher vel_pub3;
geometry_msgs::Twist vel_msg3;
ros::Publisher vel_pub4;
geometry_msgs::Twist vel_msg4;

float horizontal_distance = 4.0;
float vertical_distance = 0.75;
float speed = 1.0;

const double PI = 3.14159265359;

bool tossCoin()
{
	srand((unsigned) time(NULL));
	float coin = (float) rand()/RAND_MAX;
	if (coin > 0.5)
		return true;
	return false;
}

float setPosition(bool coin)
{
	if (coin)
		return 0;

	return (180 * M_PI) / 180;
}

void move(float distance)
{
	vel_msg.linear.x = speed;
	vel_msg2.linear.x = speed;
	vel_msg3.linear.x = speed;
	vel_msg4.linear.x = speed;
	double t0 = ros::Time::now().toSec();
	float current_distance = 0.0;
	while (current_distance < distance)
	{
		vel_pub.publish(vel_msg);
		vel_pub2.publish(vel_msg2);
		vel_pub3.publish(vel_msg3);
		vel_pub4.publish(vel_msg4);
		double t1 = ros::Time::now().toSec();
		current_distance = speed * (t1 - t0);   
	}

	vel_msg.linear.x = 0;
	vel_msg2.linear.x = 0;
	vel_msg3.linear.x = 0;
	vel_msg4.linear.x = 0;
	vel_pub.publish(vel_msg);
	vel_pub2.publish(vel_msg2);
	vel_pub3.publish(vel_msg3);
	vel_pub4.publish(vel_msg4);
}

void turn(double turnspeed, double angle)
{
	double angular_speed = (turnspeed * PI) / 180;
	double relative_angle = (angle * PI) / 180;
	vel_msg.angular.z = std::abs(angular_speed);
	vel_msg2.angular.z = std::abs(angular_speed);
	vel_msg3.angular.z = std::abs(angular_speed);
	vel_msg4.angular.z = std::abs(angular_speed);
	double t0 = ros::Time::now().toSec();
	double current_angle = 0.0;
	while (current_angle < relative_angle)
	{
		vel_pub.publish(vel_msg);
		vel_pub2.publish(vel_msg2);
		vel_pub3.publish(vel_msg3);
		vel_pub4.publish(vel_msg4);
		double t1 = ros::Time::now().toSec();
		current_angle = angular_speed * (t1 - t0);
	}
	vel_msg.angular.z = 0;
	vel_msg2.angular.z = 0;
	vel_msg3.angular.z = 0;
	vel_msg4.angular.z = 0;
	vel_pub.publish(vel_msg);
	vel_pub2.publish(vel_msg2);
	vel_pub3.publish(vel_msg3);
	vel_pub4.publish(vel_msg4);
}

void turtleMovement()
{
	move(horizontal_distance);
	turn(15.0, 90.0);
	move(vertical_distance);
	turn(15.0, 90.0);
	horizontal_distance = horizontal_distance * 2.0;
	move(horizontal_distance);
	turn(15.0, 90.0);
	move(vertical_distance);
	turn(15.0, 90.0);
	horizontal_distance = horizontal_distance / 2.0;
	move(horizontal_distance);
}

int main(int argc, char **argv)
{
	//int nodeid = std::system(argv[1]);
	//std::stringstream ss;
	//std::cout << nodeid << std::endl;
	//ss << nodeid;
	std::string nodename = "turtle1";
	ros::init(argc, argv, "moveturtles");
	ros::NodeHandle nh;
	
	if (nodename.compare("turtle1") == 0)
	{
		int color_r, color_g, color_b;
		nh.getParam("r", color_r);
		nh.getParam("g", color_g);
		nh.getParam("b", color_b);

		ros::service::waitForService("clear");

		ros::param::set("background_r", color_r);
		ros::param::set("background_g", color_g);
		ros::param::set("background_b", color_b);

		ros::ServiceClient clearclient = nh.serviceClient<std_srvs::Empty>("clear");
		std_srvs::Empty srv;
		clearclient.call(srv);
		ros::ServiceClient spawnclient = nh.serviceClient<turtlesim::Spawn>("spawn");
		turtlesim::Spawn::Request req2;
		turtlesim::Spawn::Response resp2;
		req2.x = 5.544445;
		req2.y = 9.4445;
		req2.theta = setPosition(tossCoin());
		req2.name = "turtle2";
		turtlesim::Spawn::Request req3;
		turtlesim::Spawn::Response resp3;
		req3.x = 5.544445;
		req3.y = 7.4445;
		req3.theta = setPosition(tossCoin());
		req3.name = "turtle3";
		turtlesim::Spawn::Request req4;
		turtlesim::Spawn::Response resp4;
		req4.x = 5.544445;
		req4.y = 3.4445;
		req4.theta = setPosition(tossCoin());
		req4.name = "turtle4";
		spawnclient.call(req2, resp2);
		spawnclient.call(req3, resp3);
		spawnclient.call(req4, resp4);
	}

	vel_pub = nh.advertise<geometry_msgs::Twist>(nodename+"/cmd_vel", 10);
	vel_pub2 = nh.advertise<geometry_msgs::Twist>("turtle2/cmd_vel", 10);
	vel_pub3 = nh.advertise<geometry_msgs::Twist>("turtle3/cmd_vel", 10);
	vel_pub4 = nh.advertise<geometry_msgs::Twist>("turtle4/cmd_vel", 10);
	turtleMovement();
	ros::spin();
	return 0;
}
