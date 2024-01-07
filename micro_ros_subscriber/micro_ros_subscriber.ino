#include <micro_ros_arduino.h>

#include <M5Stack.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>

rcl_subscription_t subscriber;
std_msgs__msg__Int32 msg;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn;}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}


void subscription_callback(const void * msgin)
{  
  // TODO M5stack
  // M5.Lcd.printf("micro ROS2 Data:%d \n",msg.data);

  if (msg.data == 0){
    M5.Lcd.drawJpgFile(SD, "/img/green_status.jpg");  
  }
  else if (msg.data == 1){
    M5.Lcd.drawJpgFile(SD, "/img/yellow_status.jpg");
  }
  else if (msg.data == 2){
    M5.Lcd.drawJpgFile(SD, "/img/blue_status.jpg");
  }

  else if (msg.data == 3){
    M5.Lcd.drawJpgFile(SD, "/img/red_status.jpg");
  }

}

void setup() {
  // TODO M5stack
  M5.begin();
  M5.Lcd.setTextSize(2);
  M5.Lcd.print("micro ROS2 M5Stack START\n");

  set_microros_transports();
  
  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_arduino_node", "", &support));

  // create subscriber
  RCCHECK(rclc_subscription_init_default(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "/micro_ros_arduino_data"));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg, &subscription_callback, ON_NEW_DATA));
}

void loop() {
  delay(100);
  RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}