#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class Talker : public rclcpp::Node
{
public:
    Talker() : Node("talker"), count_(1)
    {
        rclcpp::QoS qos(rclcpp::KeepLast(10));
        qos.deadline(rclcpp::Duration::from_seconds(1.0));

        publisher_ = this->create_publisher<std_msgs::msg::String>("chatter", qos);

        timer_ = this->create_wall_timer(
            900ms,
            std::bind(&Talker::timer_callback, this)
        );
    }

private:
    void timer_callback()
    {
        std_msgs::msg::String msg;
        msg.data = "hello " + std::to_string(count_++);

        RCLCPP_INFO(this->get_logger(), "I sent: %s", msg.data.c_str());
        publisher_->publish(msg);
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    int count_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Talker>());
    rclcpp::shutdown();
    return 0;
}
