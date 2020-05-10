/// Point Publisher

////////////////////
/// DEPENDENCIES ///
////////////////////

// ROS 2
#include <rclcpp/rclcpp.hpp>

// ROS 2 interfaces
#include <geometry_msgs/msg/point_stamped.hpp>

// Miscellaneous
#include <chrono>

/////////////////
/// CONSTANTS ///
/////////////////

/// The name of this node
const std::string NODE_NAME = "point_publisher";

//////////////////
/// NODE CLASS ///
//////////////////

/// Class representation of this node
class PointPublisher : public rclcpp::Node
{
public:
  /// Constructor
  PointPublisher();

private:
  /// Timer
  rclcpp::TimerBase::SharedPtr timer_;
  /// Publisher of the points
  rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr pub_point_;

  /// Callback for the timer
  void timer_callback();
};

PointPublisher::PointPublisher() : Node(NODE_NAME)
{
  // Parameters
  double frequency = this->declare_parameter<double>("frequency", 1.0);
  this->declare_parameter<std::string>("frame_id", "world");
  this->declare_parameter<double>("position.x", 0.0);
  this->declare_parameter<double>("position.y", 0.0);
  this->declare_parameter<double>("position.z", 0.0);

  // Register publisher
  rclcpp::QoS qos_point = rclcpp::QoS(rclcpp::QoSInitialization::from_rmw(rmw_qos_profile_default));
  pub_point_ = this->create_publisher<geometry_msgs::msg::PointStamped>("point", qos_point);

  // Setup timer
  timer_ = this->create_wall_timer(std::chrono::duration<double>(1.0 / frequency), std::bind(&PointPublisher::timer_callback, this));

  RCLCPP_INFO(this->get_logger(), "Node initialised");
}

void PointPublisher::timer_callback()
{
  // Create output msg
  geometry_msgs::msg::PointStamped msg_point;

  // Fill msg data
  msg_point.header.stamp = rclcpp::Clock().now();
  msg_point.header.frame_id = this->get_parameter("frame_id").get_value<std::string>();
  msg_point.point.x = this->get_parameter("position.x").get_value<double>();
  msg_point.point.y = this->get_parameter("position.y").get_value<double>();
  msg_point.point.z = this->get_parameter("position.z").get_value<double>();

  // Publish msg
  pub_point_->publish(msg_point);
}

////////////
/// MAIN ///
////////////

/// Main function that initiates an object of `PointPublisher` class as the core of this node.
int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PointPublisher>());
  rclcpp::shutdown();
  return EXIT_SUCCESS;
}
