#ifndef _ASTRA_H
#define _ASTRA_H

#ifndef Q_MOC_RUN
#include <ros/ros.h>

#include <iostream>

#include <image_transport/image_transport.h>
#include <image_transport/subscriber_filter.h>

#include <cv_bridge/cv_bridge.h>

#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/PointCloud2.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

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

#include <stdio.h>

#include <boost/foreach.hpp>

#include <tf/transform_listener.h>
#include <tf_conversions/tf_eigen.h>

#include <sensor_msgs/LaserScan.h>
#endif

using namespace message_filters;

class Astra_Camera{

public:

  typedef sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::PointCloud2> syncPolicy;
  
  Astra_Camera(ros::NodeHandle n);
  ~Astra_Camera();
  std_msgs::Header header;
  std::string encoding;

  void RgbCallback(const sensor_msgs::ImageConstPtr& msg_rgb);
  void DepthCallback(const sensor_msgs::ImageConstPtr& msg_depth );
  void DepthViewCallback(const sensor_msgs::ImageConstPtr& msg_depth );
  void PointCB(const sensor_msgs::ImageConstPtr &msg_rgb, const sensor_msgs::PointCloud2ConstPtr &msg);
  void PointCB2(const sensor_msgs::PointCloud2ConstPtr& msg);
  void PointCallback(const sensor_msgs::PointCloud2ConstPtr& msg);
  void saveFiles();
  cv::Mat rgb, depth, depthView;
  sensor_msgs::PointCloud2* astraPoints;
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr astraCloud;
  std::vector<float> depthData;
  ros::Publisher pubC;
  float* points;
  int getStatus();
  void setStatus(int);
  
private:
  int status;
  ros::NodeHandle n_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber astra_sub;
  image_transport::SubscriberFilter astra_sub2,astra_sub3;
  ros::Subscriber point_sub, p2;
  message_filters::Subscriber<sensor_msgs::PointCloud2> ptc_sub;
  message_filters::Subscriber<sensor_msgs::Image> rgb_sub;
  message_filters::Synchronizer< syncPolicy > sync;
};

#endif
