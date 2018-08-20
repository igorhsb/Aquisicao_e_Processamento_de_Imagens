#include "../include/project_pkg/sync.h"

/*Syncronize_Devices::Syncronize_Devices(ros::NodeHandle n, ros::NodeHandle n2):
  n_(n),
  it_(n),
  n2_(n2),
  laser_sub_(n_, "base_scan", 10),
  laser_notifier_(laser_sub_,listener_, "laser", 10),
  //astraPTC_sub(n_, "/zed/point_cloud/cloud_registered", 100),
  astraPTC_sub(n_, "/camera/depth_registered/points", 100),
  laser_sub(n_, "/scan", 100),
  imu_sub0(n_, "/imu_data_str", 100),
  //imu1_sub(n_, "", 100),
  sync( syncPolicy( 10 ), astraPTC_sub, laser_sub),
  astraCloud(new pcl::PointCloud<pcl::PointXYZRGBA>),
  laserCloud(new pcl::PointCloud<pcl::PointXYZRGBA>)  
{
	//sync.registerCallback(boost::bind(&Syncronize_Devices::Callback,this, _1, _2));
	astraRGB_sub = it_.subscribe("/camera/rgb/image_rect_color", 1, &Syncronize_Devices::RgbCallback, this);
	scan_sub = n_.subscribe("/scan", 1, &Syncronize_Devices::scanCallback, this);
	//point_sub = n_.subscribe("/camera/depth_registered/points", 1, &Syncronize_Devices::AstraCB, this);
}


void Syncronize_Devices::RgbCallback(const sensor_msgs::ImageConstPtr& msg_rgb)
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
  //std::cout << "RGB" << std::endl;
}

void Syncronize_Devices::Callback( const sensor_msgs::PointCloud2ConstPtr &msg, const sensor_msgs::LaserScanConstPtr& scan_in){       
    
    std::cout << "Fine" << std::endl;
	
  	// ASTRA PointCloud
  	pcl::fromROSMsg(*msg, *astraCloud);
	 // Remover NaN se existir
  	std::vector<int> indicesNAN;
	removeNaNFromPointCloud(*astraCloud, *astraCloud, indicesNAN);
	astra_on = true;
	
	// LASER Data
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
        laserCloud->points[index].x = cloud.points[index].x;
        laserCloud->points[index].y = cloud.points[index].y;
        laserCloud->points[index].z = cloud.points[index].z;
        laserCloud->points[index].rgb = 255;    
    }
    std::vector<int> indicesNAN2;
	removeNaNFromPointCloud(*laserCloud, *laserCloud, indicesNAN2);
	laser_on = true;
}

void Syncronize_Devices::scanCallback (const sensor_msgs::LaserScan::ConstPtr& scan_in)
{
//pthread_mutex_lock(&mutexL);
	std::cout << "laser" << std::endl;
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

        laserCloud->points[index].x = cloud.points[index].x;
        laserCloud->points[index].y = cloud.points[index].y;
        laserCloud->points[index].z = cloud.points[index].z;
        laserCloud->points[index].rgb = 255;
        
    }
    std::vector<int> indicesNAN;
	//removeNaNFromPointCloud(*laserCloud, *laserCloud, indicesNAN);
//   pthread_mutex_unlock(&mutexL);
}


void Syncronize_Devices::AstraCB(const sensor_msgs::PointCloud2ConstPtr& msg){
	std::cout << "Astra" << std::endl;
	pcl::fromROSMsg(*msg, *astraCloud);
	 // Remover NaN se existir
  	std::vector<int> indicesNAN;
	removeNaNFromPointCloud(*astraCloud, *astraCloud, indicesNAN);
	points = (float*)astraCloud->points.data();
	astra_on = true;
}
*/

