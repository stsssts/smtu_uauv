#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <sensor_msgs/Joy.h>

class TeleopNode
{
public:
  TeleopNode();

private:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);

  ros::NodeHandle nh_;

  int forward_, updown_, pitch_, yaw_;
  double l_scale_, a_scale_;
  ros::Publisher vel_pub_;
  ros::Subscriber joy_sub_;
};

TeleopNode::TeleopNode() : pitch_(1), yaw_(0), forward_(2), updown_(4)
{
  nh_.param("teleop/joy_pitch", pitch_, pitch_);
  nh_.param("teleop/joy_yaw", yaw_, yaw_);
  nh_.param("teleop/joy_forward", forward_, forward_);
  nh_.param("teleop/joy_updown", updown_, updown_);

  vel_pub_ =
      nh_.advertise<sensor_msgs::JointState>("/uauv/thruster_command", 1);

  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 10,
                                             &TeleopNode::joyCallback, this);
}

void TeleopNode::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  sensor_msgs::JointState joint_command;

  joint_command.name.push_back("v_front");
  joint_command.position.push_back(-15 * joy->axes[updown_] - 30 * joy->axes[pitch_]);

  joint_command.name.push_back("v_back");
  joint_command.position.push_back(-15 * joy->axes[updown_] + 30 * joy->axes[pitch_]);

  joint_command.name.push_back("h_left");
  joint_command.position.push_back(30 * joy->axes[forward_] - 30 * joy->axes[yaw_]);

  joint_command.name.push_back("h_right");
  joint_command.position.push_back(30 * joy->axes[forward_] + 30 * joy->axes[yaw_]);

  //  joint_command.position.
  //  twist.angular.z = a_scale_ * joy->axes[angular_];
  //  twist.linear.x = l_scale_ * joy->axes[linear_];
  vel_pub_.publish(joint_command);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "teleop_node");
  TeleopNode teleop;

  ros::spin();
}
