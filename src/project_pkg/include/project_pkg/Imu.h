#ifndef Q_MOC_RUN
#include "ros/ros.h"
#include "tf/transform_listener.h"
#include "sensor_msgs/PointCloud.h"
#include "tf/message_filter.h"
#include "message_filters/subscriber.h"
#include "laser_geometry/laser_geometry.h"
#include "sensor_msgs/Image.h"
#include "sensor_msgs/Imu.h"
#include "std_msgs/String.h"
#endif
#include "Kinect.h"
#include <string>
#include "sync.h"
#include<stdio.h>
#include<fstream>
#include<iomanip>
#include<iostream> 

typedef struct orientation_struct{
  double x,y,z,w;
}orientation_data;
typedef struct vel_struct{
  double x,y,z,covariance;
}Vel_data;

typedef struct Imu_struct{
  orientation_data orientation;
  Vel_data angular_vel;
  Vel_data linear_vel;
  double pitch, roll, yaw;
}Imu_Data;

class ImuSensor{
public:
  int getStatus();
  void setStatus(int);
  ros::NodeHandle n_;
  ros::Subscriber imu_sub, imu_sub2;
  sensor_msgs::PointCloud cloud;
  Imu_Data dadosImu;
  ImuSensor(ros::NodeHandle n);
  void Callback(const sensor_msgs::Imu::ConstPtr& imu_msg);
  void Callback2(const std_msgs::String::ConstPtr& imu_msg );
  void SaveFile();
  static void* fileThreadFunc(void*);
  pthread_t fileThread;
private:
	int status;
};
