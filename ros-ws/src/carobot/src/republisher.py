#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState
from std_msgs.msg import Float64

class JointStateEnricher(Node):
    def __init__(self):
        super().__init__('joint_state_enricher')
        self.subscription = self.create_subscription(
            JointState,
            '/joint_states',
            self.joint_state_callback,
            10)
        self.publisher = self.create_publisher(
            JointState,
            '/world/empty/model/carobot/joint_states',
            10)
        self.jpublisher = [self.create_publisher(
            Float64,
            f'/world/empty/model/carobot/joint_states/J{i}',
            10) for i in range(1, 4)]

    def joint_state_callback(self, msg):
        enriched_msg = JointState()
        enriched_msg.header = msg.header
        enriched_msg.name = msg.name
        enriched_msg.position = msg.position

        num_joints = len(msg.name)
        enriched_msg.velocity = [0.0] * num_joints
        enriched_msg.effort = [0.0] * num_joints

        self.publisher.publish(enriched_msg)
        #self.get_logger().info(f'Enriched and published JointState')

        if num_joints > 0:
            float_msg = Float64()
            for i in range(num_joints):
                float_msg.data = msg.position[i]
                self.jpublisher[i].publish(float_msg)
                # self.get_logger().info(f'Published Float64: {float_msg.data}')
        self.get_logger().info(f'Published JointState with {num_joints} joints')

def main(args=None):
    rclpy.init(args=args)
    joint_state_enricher = JointStateEnricher()
    rclpy.spin(joint_state_enricher)
    joint_state_enricher.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
