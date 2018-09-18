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

#include <time.h>

#endif


extern pthread_mutex_t mutex,mutexL,mutexI,mutexZ;
extern pthread_cond_t cond1,cond2,cond3,cond4;

extern bool syncronize_devices;
extern bool zed_new_pointcloud;

extern int dispCB, dispCB2, vModCB, VModCB2, exitCond, saveSignal, syncCond;
extern bool changeBTN, imu_on, situation_change, laser_on, astra_on, zed_on;
extern QString situation;
extern float roll, pitch, yaw, imu_x, imu_y, imu_z, init_x, init_y, init_z, init_yaw, init_roll, init_pitch;
extern clock_t ck1, ck2, ck3, ck4, ck5, ckT;
extern double time1, time2, time3, time4, time5, timeT;


#endif
