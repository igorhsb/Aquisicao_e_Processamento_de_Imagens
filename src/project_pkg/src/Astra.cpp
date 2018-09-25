#include "../include/project_pkg/Astra.h"
#include <iostream>
#include <boost/thread/thread.hpp>
#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>
#include <stdio.h>
#include <stdlib.h>

bool astra_on = false;

Astra_Camera::Astra_Camera(ros::NodeHandle n):
  n_(n),
  it_(n),
  astraCloud(new pcl::PointCloud<pcl::PointXYZRGB>),
  astra_sub2(it_,"/camera/rgb/image_rect_color",100),
  astra_sub3(it_,"/camera/depth/image_rect",100),
  ptc_sub(n_, "/camera/depth_registered/points", 100),
  rgb_sub(n_,"/camera/rgb/image_rect_color",100),
  sync( syncPolicy( 10 ), rgb_sub, ptc_sub),
  status(0)
{
  astra_sub = it_.subscribe("/camera/rgb/image_rect_color", 1, &Astra_Camera::RgbCallback, this);
  //astra_sub2 = it_.subscribe("/camera/depth/image_rect", 1, &Astra_Camera::DepthCallback, this);
  //astra_sub3 = it_.subscribe("/camera/depth/image", 1, &Astra_Camera::DepthViewCallback, this);
  point_sub = n_.subscribe("/camera/depth_registered/points", 1, &Astra_Camera::PointCB2, this);
  //pubC = n_.advertise<pcl::PointCloud<pcl::PointXYZRGBA>> ("output", 1);
  //p2 = n_.subscribe("/camera/depth_registered/points", 1, &Astra_Camera::PointCallback, this);
  //astra_sub = n_.subscribe("/camera/depth/points", 1, &Astra_Camera::depthCall, this);
  
  //message_filters::Subscriber<sensor_msgs::PointCloud2> ptc_sub(n_, "/camera/depth_registered/points", 100);
  //message_filters::Subscriber<sensor_msgs::Image> lsr_sub(n_, "/camera/rgb/image_rect_color", 100);
  //Synchronizer<syncPolicy> sync(syncPolicy(100), ptc_sub, lsr_sub);
  //sync.registerCallback(boost::bind(&Astra_Camera::PointCB,this, _1, _2));
}

Astra_Camera::~Astra_Camera(){

}

void Astra_Camera::RgbCallback(const sensor_msgs::ImageConstPtr& msg_rgb)
{
  cv_bridge::CvImagePtr rgb_ptr;
  try
  {
    rgb_ptr = cv_bridge::toCvCopy(msg_rgb, sensor_msgs::image_encodings::BGR8);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }
  rgb = rgb_ptr->image;
  astra_on = true;
//  std::cout << "Work 11111" << std::endl;
  if(getStatus() == 1){
  	this->astra_sub.shutdown();
  	astra_on = false;	 		
  }
}

void Astra_Camera::DepthCallback(const sensor_msgs::ImageConstPtr& msg_depth)
{
  cv_bridge::CvImagePtr depth_ptr;
  try
  {
    depth_ptr = cv_bridge::toCvCopy(msg_depth, sensor_msgs::image_encodings::TYPE_32FC1);
  }
  catch (cv_bridge::Exception& e)
  {

    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }
  depth = depth_ptr->image;
  float z_raw = depth.data[320*240];
  //ROS_INFO("%f",z_raw);
}

void Astra_Camera::DepthViewCallback(const sensor_msgs::ImageConstPtr& msg_depthV){
  cv_bridge::CvImagePtr depthV_ptr;
  try
  {
    depthV_ptr = cv_bridge::toCvCopy(msg_depthV, sensor_msgs::image_encodings::TYPE_32FC1);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }
  depthView = depthV_ptr->image;
}

boost::shared_ptr<pcl::visualization::PCLVisualizer> rgbVis (pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr cloud)
{
  // --------------------------------------------
  // -----Open 3D viewer and add point cloud-----
  // --------------------------------------------
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));
  viewer->setBackgroundColor (0, 0, 0);
  pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(cloud);
  viewer->addPointCloud<pcl::PointXYZRGB> (cloud, rgb, "sample cloud");
  viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "sample cloud");
  viewer->addCoordinateSystem (1.0);
  viewer->initCameraParameters ();
  return (viewer);
}

int atsa = 0;

void Astra_Camera::PointCB(const sensor_msgs::ImageConstPtr &msg_rgb, const sensor_msgs::PointCloud2ConstPtr &msg){   
    
    // Image RGB
    cv_bridge::CvImagePtr rgb_ptr;
    try
  	{
    	rgb_ptr = cv_bridge::toCvCopy(msg_rgb, sensor_msgs::image_encodings::BGR8);
  	}
  	catch (cv_bridge::Exception& e)
  	{
    	ROS_ERROR("cv_bridge exception: %s", e.what());
    	return;
  	}
  	rgb = rgb_ptr->image;
  	
  	// PointCloud
  	pcl::fromROSMsg(*msg, *astraCloud);
	 // Remover NaN se existir
  	std::vector<int> indicesNAN;
	removeNaNFromPointCloud(*astraCloud, *astraCloud, indicesNAN);
	points = (float*)astraCloud->points.data();
	std::cout << "Work 22222" << std::endl;
	astra_on = true;
	if(getStatus() == 1){
   		this->point_sub.shutdown();
   		astra_on = false;	
   	}
}

void Astra_Camera::PointCB2(const sensor_msgs::PointCloud2ConstPtr& msg){
	pcl::fromROSMsg(*msg, *astraCloud);
	 // Remover NaN se existir
  	std::vector<int> indicesNAN;
	removeNaNFromPointCloud(*astraCloud, *astraCloud, indicesNAN);
	points = (float*)astraCloud->points.data();
	astra_on = true;
}

void Astra_Camera::saveFiles(){
	//pcl::io::savePCDFileASCII ("test_pcd.pcd", astraCloud);
      pcl::io::savePCDFileASCII("astra_cloud.pcd", *astraCloud);
      cout << "astra files generate" << endl;
}

int Astra_Camera::getStatus()
{
	return status;
}

void Astra_Camera::setStatus(int val)
{
	status = val;
}


