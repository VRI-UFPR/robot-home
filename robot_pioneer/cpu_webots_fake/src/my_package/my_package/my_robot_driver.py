#!/usr/bin/env python3

# =============================================================================
#  Header
# =============================================================================

import rclpy
from geometry_msgs.msg import Twist

HALF_DISTANCE_BETWEEN_WHEELS = 0.045
WHEEL_RADIUS = 0.025

# =============================================================================
#  MyRobotDriver
# =============================================================================

class MyRobotDriver:
    def init(self, webots_node, properties):
        self.__robot = webots_node.robot

        self.__left_motor = self.__robot.getDevice('left wheel')
        self.__right_motor = self.__robot.getDevice('right wheel')

        self.__left_motor.setPosition(float('inf'))
        self.__left_motor.setVelocity(0)

        self.__right_motor.setPosition(float('inf'))
        self.__right_motor.setVelocity(0)

        self.__target_twist = Twist()

        rclpy.init(args=None)
        self.__node = rclpy.create_node('my_robot_driver')
        self.__node.create_subscription(Twist, 'cmd_vel', self.__cmd_vel_callback, 1)

    def __cmd_vel_callback(self, twist):
        self.__target_twist = twist

    def step(self):
        rclpy.spin_once(self.__node, timeout_sec=0)

        forward_speed = self.__target_twist.linear.x
        angular_speed = self.__target_twist.angular.z

        command_motor_left = (forward_speed - angular_speed * HALF_DISTANCE_BETWEEN_WHEELS) / WHEEL_RADIUS
        command_motor_right = (forward_speed + angular_speed * HALF_DISTANCE_BETWEEN_WHEELS) / WHEEL_RADIUS

        self.__left_motor.setVelocity(command_motor_left)
        self.__right_motor.setVelocity(command_motor_right)

# =============================================================================
#  Documentation
# =============================================================================

# O codigo já publica os dispotivos do robo

# Topicos
"""
/cmd_vel
/parameter_events
/pioneer/RPlidar_A2
/pioneer/RPlidar_A2/point_cloud
/pioneer/green_led
/pioneer/kinect_color/camera_info
/pioneer/kinect_range/camera_info
/pioneer/kinect_range/image
/pioneer/kinect_range/point_cloud
/pioneer/lower_yellow_led
/pioneer/red_led_1
/pioneer/red_led_2
/pioneer/red_led_3
/pioneer/so0
/pioneer/so1
/pioneer/so10
/pioneer/so11
/pioneer/so12
/pioneer/so13
/pioneer/so14
/pioneer/so15
/pioneer/so2
/pioneer/so3
/pioneer/so4
/pioneer/so5
/pioneer/so6
/pioneer/so7
/pioneer/so8
/pioneer/so9
/pioneer/white_led
/pioneer/yellow_led
/remove_urdf_robot
/rosout
"""