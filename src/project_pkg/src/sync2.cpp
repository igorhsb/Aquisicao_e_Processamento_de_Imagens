#include "../include/project_pkg/sync2.h"


SyncronizeDevices::SyncronizeDevices(ros::NodeHandle n) :
    n_(n),
    it_(n),
    laser_sub_(n_, "/base_scan", 10),
    laser_notifier_(laser_sub_,listener_, "laser", 10),
    laserCloud(new pcl::PointCloud<pcl::PointXYZRGBA>),
    astraCloud(new pcl::PointCloud<pcl::PointXYZRGBA>),
    astraPTC_sub(n_, "/camera/depth_registered/points", 100),
    laser_sub(n_, "/scan", 100),
    sync( syncPolicy( 10 ), laser_sub, astraPTC_sub)
{
	int s = 729*6;
	laserCloud->points.resize(729);
	laserPoints = (float*) malloc(s*sizeof(float));
	sync.registerCallback(boost::bind(&SyncronizeDevices::scanCallback,this, _1, _2));
	astraRGB_sub = it_.subscribe("/camera/rgb/image_rect_color", 1, &SyncronizeDevices::RgbCallback, this);
	//imu_sub2 = n_.subscribe("/imu_data_str", 1, &ImuSensor::Callback2, this);
}

SyncronizeDevices::~SyncronizeDevices(){
	free(laserPoints);
}

void SyncronizeDevices::scanCallback (const sensor_msgs::LaserScan::ConstPtr& scan_in , const sensor_msgs::PointCloud2ConstPtr &msg)
{
	
    try
    {
        projector_.transformLaserScanToPointCloud("laser",*scan_in, cloud,listener_);
    }
    catch (tf::TransformException& e)
    {
        std::cout << e.what();
        return;
    }
    int index, i=0;
    for(index=0;index<729;index++){
        laserPoints[i] = cloud.points[index].x;
        laserPoints[i + 1] = cloud.points[index].y;
        laserPoints[i + 2] = cloud.points[index].z;
        laserPoints[i + 3] = 255*255*260;
        laserPoints[i + 4] = scan_in->ranges[index];
        laserPoints[i + 5] = (-135+(0.35*index));
        i+=6;   
        laserCloud->points[index].x = cloud.points[index].x;
        laserCloud->points[index].y = cloud.points[index].y;
        laserCloud->points[index].z = cloud.points[index].z;
        laserCloud->points[index].rgb = 255;
    }
	std::vector<int> indicesNAN2;
	removeNaNFromPointCloud(*laserCloud, *laserCloud, indicesNAN2);
	
	// ASTRA PointCloud
  	pcl::fromROSMsg(*msg, *astraCloud);
	 // Remover NaN se existir
  	std::vector<int> indicesNAN;
	removeNaNFromPointCloud(*astraCloud, *astraCloud, indicesNAN);
	
}

void SyncronizeDevices::RgbCallback(const sensor_msgs::ImageConstPtr& msg_rgb)
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
  //std::cout << "RGB" << std::endl;
}
