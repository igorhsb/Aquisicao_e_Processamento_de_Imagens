#ifndef Q_MOC_RUN
#include "ros/ros.h"
#include "tf/transform_listener.h"
#include "sensor_msgs/PointCloud.h"
#include "tf/message_filter.h"

#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <message_filters/synchronizer.h>

#include "laser_geometry/laser_geometry.h"
#include "sensor_msgs/Image.h"

#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/range_image/range_image_planar.h>
#include <pcl/common/common_headers.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h>
#include <pcl_ros/transforms.h>
#include <pcl/filters/passthrough.h>
#include <pcl/features/normal_3d.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>

#include <boost/foreach.hpp>

#include <image_transport/image_transport.h>
#include <image_transport/subscriber_filter.h>

#include "sensor_msgs/Image.h"

#include "sensor_msgs/Imu.h"

#include "std_msgs/String.h"

#include <cv_bridge/cv_bridge.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <sensor_msgs/image_encodings.h>

#endif

#include "Kinect.h"
#include "sync.h"
#include<stdio.h>
#include<fstream>
#include<iomanip>
#include<iostream>


typedef struct Imu_struct_data{
  double pitch, roll, yaw;
}imu_data;

using namespace message_filters;


class SyncronizeDevices{

public:

  typedef sync_policies::ApproximateTime< sensor_msgs::LaserScan, sensor_msgs::PointCloud2 > syncPolicy;

  ros::NodeHandle n_;
  ros::Subscriber scan_sub, imu_sub;
  sensor_msgs::PointCloud cloud;
  laser_geometry::LaserProjection projector_;
  tf::TransformListener listener_;
  message_filters::Subscriber<sensor_msgs::LaserScan> laser_sub_;
  tf::MessageFilter<sensor_msgs::LaserScan> laser_notifier_;
  ros::Publisher scan_pub_;
  message_filters::Subscriber<sensor_msgs::PointCloud2> astraPTC_sub;
  message_filters::Subscriber<sensor_msgs::LaserScan> laser_sub;
  message_filters::Synchronizer< syncPolicy > sync;
  imu_data dadosImu;
  cv::Mat rgb;
  image_transport::ImageTransport it_;
  image_transport::Subscriber astraRGB_sub;
  
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr astraCloud;
  float* points;
  LaserData laserData[729];
  float *laserPoints;

  SyncronizeDevices(ros::NodeHandle n);
  ~SyncronizeDevices();
  void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan_in, const sensor_msgs::PointCloud2ConstPtr &msg);
  void RgbCallback(const sensor_msgs::ImageConstPtr& msg_rgb);
  void Callback2(const std_msgs::String::ConstPtr& imu_msg );
  
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr laserCloud;
  
  int getStatus();
  void setStatus(int);
  
  void SaveFile();
  
  private:
  
  int status;
  static void* fileThreadFunc(void*);
  pthread_t fileThread;
  
};
