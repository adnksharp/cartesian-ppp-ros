#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64

class JointStatePublisher(Node):
    def __init__(self):
        super().__init__('robot_joint_position_publisher')

        self.publisher = [self.create_publisher(
            Float64,
            f'/world/empty/model/carobot/joint/prismatic_{i}_joint/cmd_pos',
            10
        ) for i in range(4)]

        timer_period = 0.1
        self.timer = self.create_timer(timer_period, self.timer_callback)

    def timer_callback(self):
        msg = Float64()

        try:
            try:
                input_srt = input('>> ')
            except KeyboardInterrupt:
                self.get_logger().fatal(f'{"X # " * 20}')
                exit(0)
            pos = list(map(float, input_srt.split(',')))
            if len(pos) == 3:
                for i in range(3):
                    msg.data = pos[i]
                    self.publisher[i].publish(msg)
                self.get_logger().info(f'{pos}')
        except:
            pass

def main(args=None):
    rclpy.init(args=args)
    joint_state_enricher = JointStatePublisher()
    rclpy.spin(joint_state_enricher)
    joint_state_enricher.destroy_node()

if __name__ == '__main__':
    main()
