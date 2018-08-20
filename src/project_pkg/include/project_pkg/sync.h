#ifndef _SYNC_H_
#define _SYNC_H_

#include <pthread.h>
#include <iostream>
#include <QMainWindow>

#ifndef Q_MOC_RUN
#include <ros/ros.h>

#include <iostream>

#include <image_transport/image_transport.h>
#include <image_transport/subscriber_filter.h>

#include "tf/transform_listener.h"
#include "tf/message_filter.h"

#include "laser_geometry/laser_geometry.h"

#include "sensor_msgs/Image.h"

#include "sensor_msgs/Imu.h"

#include "sensor_msgs/PointCloud.h"

#include "std_msgs/String.h"

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
#include <pcl/features/normal_3d.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>

#include <boost/foreach.hpp>

#include <tf/transform_listener.h>
#include <tf_conversions/tf_eigen.h>

#include <sensor_msgs/LaserScan.h>

#include <iostream>
#include <boost/thread/thread.hpp>
#include <stdio.h>
#include <stdlib.h>
#endif


extern pthread_mutex_t mutex,mutexL,mutexI,mutexZ;
extern pthread_cond_t cond1,cond2,cond3,cond4;

extern bool syncronize_devices;
extern bool zed_new_pointcloud;

extern int dispCB, dispCB2, vModCB, VModCB2, exitCond, saveSignal, syncCond;
extern bool changeBTN, imu_on, situation_change, laser_on, astra_on, zed_on;
extern QString situation;
extern float roll, pitch, yaw, imu_x, imu_y, imu_z, init_x, init_y, init_z, init_yaw, init_roll, init_pitch;

/*using namespace message_filters;

class Syncronize_Devices{

public:

  typedef sync_policies::ApproximateTime< sensor_msgs::PointCloud2, sensor_msgs::LaserScan > syncPolicy;
  
  Syncronize_Devices(ros::NodeHandle n, ros::NodeHandle n2);
  ~Syncronize_Devices();
  //, const std_msgs::StringConstPtr& imu_msg0
  void Callback(const sensor_msgs::PointCloud2ConstPtr &msg, const sensor_msgs::LaserScanConstPtr& scan_in);
  void RgbCallback(const sensor_msgs::ImageConstPtr& msg_rgb);
  void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan_in);
  void AstraCB(const sensor_msgs::PointCloud2ConstPtr& msg);
  
  cv::Mat rgb;
  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr astraCloud;
  float* points;
  
  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr laserCloud;
  sensor_msgs::PointCloud cloud;
  laser_geometry::LaserProjection projector_;
  tf::TransformListener listener_;
  tf::MessageFilter <sensor_msgs::LaserScan> laser_notifier_;
  message_filters::Subscriber<sensor_msgs::LaserScan> laser_sub_;
  
  ros::NodeHandle n_, n2_;
  ros::Subscriber scan_sub, point_sub;
  image_transport::ImageTransport it_;
  image_transport::Subscriber astraRGB_sub;
  message_filters::Subscriber<sensor_msgs::PointCloud2> astraPTC_sub;
  message_filters::Subscriber<sensor_msgs::LaserScan> laser_sub;
  message_filters::Subscriber<std_msgs::String> imu_sub0;
  message_filters::Subscriber<sensor_msgs::Imu> imu1_sub;
  message_filters::Synchronizer< syncPolicy > sync;
};
*/

#endif
