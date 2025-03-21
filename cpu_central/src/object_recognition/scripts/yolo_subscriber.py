#!/usr/bin/env python3

import cv2
import threading
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
from yolo_msgs.msg import ModelResults

class yolo_subscriber(Node):

    def __init__(self):
        super().__init__('yolo_subscriber')

	# create subscription to webcam image and model results
        self.subscription = self.create_subscription(Image, '/webcam_image', self.camera_callback, 10)
        self.subscription = self.create_subscription(ModelResults, '/model_results', self.yolo_callback, 10)
        self.cnt = 0


        self.bridge = CvBridge()


    def camera_callback(self, data):
        global img
        img = self.bridge.imgmsg_to_cv2(data, "bgr8")
        


    def yolo_callback(self, data):
        global img	

        for r in data.model_results:
            # print info
            self.get_logger().info(f"Object {self.cnt}: {r.class_name} at ({r.top}, {r.left}) ({r.bottom}, {r.right})")
            # put box in cam
            cv2.rectangle(img, (r.top, r.left), (r.bottom, r.right), (255, 0, 255), 3)
            cv2.putText(img, r.class_name, [r.top, r.left] , cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2)

            self.cnt += 1

        self.cnt = 0
        
	# save image for testing purposes
        cv2.imwrite('inference_result.jpg', img)


if __name__ == '__main__':
    rclpy.init(args=None)
    yolo_subscriber = yolo_subscriber()

    executor = rclpy.executors.MultiThreadedExecutor()
    executor.add_node(yolo_subscriber)

    executor_thread = threading.Thread(target=executor.spin, daemon=True)
    executor_thread.start()
    
    rate = yolo_subscriber.create_rate(2)
    try:
        while rclpy.ok():
            rate.sleep()
    except KeyboardInterrupt:
        pass

    rclpy.shutdown()
    executor_thread.join()