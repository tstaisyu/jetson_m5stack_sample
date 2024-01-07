#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

using namespace std::chrono_literals;


class Int32Pub : public rclcpp::Node
{
private:
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pubInt_;
    size_t count_;
    size_t cnt_loop_;
    void TimerCallback();

public:
    Int32Pub()
      : Node("int32pub_node"), count_(0), cnt_loop_(0)
    {
      pubInt_ = this->create_publisher<std_msgs::msg::Int32>("/micro_ros_arduino_data",10);
      timer_ = this->create_wall_timer(
      500ms, std::bind(&Int32Pub::TimerCallback, this));
    }

};

void Int32Pub::TimerCallback()
{
      auto msg = std_msgs::msg::Int32();
      cnt_loop_++;
      if (cnt_loop_ >= 5){
        msg.data = count_++;
        cnt_loop_ = 0;
        if (msg.data >= 3){
          count_ = 0;
        }
      }
      else{
        msg.data = count_;
      }
      RCLCPP_INFO(this->get_logger(), "Publishing: '%d'", msg.data);
      pubInt_->publish(msg);
}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Int32Pub>());
  rclcpp::shutdown();
  return 0;
}