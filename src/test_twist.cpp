#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"

using namespace std::chrono_literals;

class ServoTwistPublisher : public rclcpp::Node
{
public:
  ServoTwistPublisher() : Node("servo_twist_publisher")
  {
    pub_ = this->create_publisher<geometry_msgs::msg::TwistStamped>(
      "/servo_node/delta_twist_cmds", 10);

    timer_ = this->create_wall_timer(
      20ms, std::bind(&ServoTwistPublisher::publish_cmd, this));  // 50 Hz
  }

private:
  void publish_cmd()
  {
    geometry_msgs::msg::TwistStamped msg;

    // REQUIRED for MoveIt Servo
    msg.header.stamp = this->get_clock()->now();
    msg.header.frame_id = "base_link";

    // Example: rotate around Z axis
    msg.twist.angular.z = 0.1;

    pub_->publish(msg);
  }

  rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ServoTwistPublisher>());
  rclcpp::shutdown();
  return 0;
}