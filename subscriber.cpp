#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;

class Listener : public rclcpp::Node
{
public:
    Listener() : Node("listener")
    {
        rclcpp::QoS qos(rclcpp::KeepLast(10));
        qos.deadline(rclcpp::Duration::from_seconds(1.0));

        rclcpp::SubscriptionOptions options;

        options.event_callbacks.deadline_callback =
            [this](rclcpp::QOSDeadlineRequestedInfo &)
        {
            RCLCPP_ERROR(this->get_logger(),
                         "🚨 PANIC: I didn’t hear from the sender!");
        };

        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "chatter",
            qos,
            std::bind(&Listener::topic_callback, this, _1),
            options
        );
    }

private:
    void topic_callback(const std_msgs::msg::String::SharedPtr msg) const
    {
        RCLCPP_INFO(this->get_logger(), "I heard: %s", msg->data.c_str());
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Listener>());
    rclcpp::shutdown();
    return 0;
}
