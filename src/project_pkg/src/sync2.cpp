#include "../include/project_pkg/sync2.h"


SyncronizeDevices::SyncronizeDevices(ros::NodeHandle n) :
    n_(n),
    it_(n),
    laser_sub_(n_, "/base_scan", 10),
    laser_notifier_(laser_sub_,listener_, "laser", 10),
    laserCloud(new pcl::PointCloud<pcl::PointXYZRGB>),
    astraCloud(new pcl::PointCloud<pcl::PointXYZRGB>),
    astraPTC_sub(n_, "/camera/depth_registered/points", 100),
    laser_sub(n_, "/scan", 100),
    status(0),
    sync( syncPolicy( 10 ), laser_sub, astraPTC_sub)
{
	int s = 729*6;
	laserCloud->points.resize(729);
	astraCloud->points.resize(640*480);
	laserPoints = (float*) malloc(s*sizeof(float));
	sync.registerCallback(boost::bind(&SyncronizeDevices::scanCallback,this, _1, _2));
	astraRGB_sub = it_.subscribe("/camera/rgb/image_rect_color", 1, &SyncronizeDevices::RgbCallback, this);
	imu_sub = n_.subscribe("/imu_data_str", 1, &SyncronizeDevices::Callback2, this);

}

SyncronizeDevices::~SyncronizeDevices(){
	free(laserPoints);
}

void SyncronizeDevices::scanCallback (const sensor_msgs::LaserScan::ConstPtr& scan_in , const sensor_msgs::PointCloud2ConstPtr &msg)
{
	if(syncCond == 1){
		pthread_mutex_lock(&mutexL);
		pthread_cond_wait(&cond1,&mutexL);	
    }
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
        laserPoints[i + 5] = (-135+(0.35*index)); // angulo
        i+=6;   
        laserCloud->points[index].x = cloud.points[index].x;
        laserCloud->points[index].y = cloud.points[index].y;
        laserCloud->points[index].z = cloud.points[index].z;
        laserCloud->points[index].rgb = 255;
    }
	std::vector<int> indicesNAN2;
	removeNaNFromPointCloud(*laserCloud, *laserCloud, indicesNAN2);
    laserCloud->width = 729;
	laserCloud->height = 1;
	
    time1 = ((double)( clock() - ck1 )) / CLOCKS_PER_SEC;
    //cout << "Zed ---> Laser" << time1 << " seconds" << endl;
	ck2 = clock();
	// ASTRA PointCloud
  	pcl::fromROSMsg(*msg, *astraCloud);
	 // Remover NaN se existir
  	std::vector<int> indicesNAN;
	removeNaNFromPointCloud(*astraCloud, *astraCloud, indicesNAN);
	
	astra_on = true;
	laser_on = true;
	
	if(syncCond == 1){
	    time2 = ((double)( clock() - ck2 )) / CLOCKS_PER_SEC;
	    //cout << "Laser ---> Astra" << time2 << " seconds" << endl;
		ck3 = clock();
		pthread_cond_signal(&cond2);
		pthread_mutex_unlock(&mutexL);
	}
	//if(getStatus() == 1)
	
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
  if(getStatus() == 1)
   		this->astraRGB_sub.shutdown();
}

void SyncronizeDevices::Callback2(const std_msgs::String::ConstPtr& imu_msg ){
    if(syncCond == 1){
	    pthread_mutex_lock(&mutexI);
	    pthread_cond_wait(&cond2,&mutexI);
	}
    std::string aux = (std::string)imu_msg->data;
    std::string r = "", p = "", y = "";
    int i;
    for(i = 45; i < 74; i ++){
    if((aux[i] <= '9' && aux[i] >= '0') || aux[i] == '.' || aux[i] == '-' )
        y += aux[i];
    }
    for(i = 76; i < 97; i ++){
    if((aux[i] <= '9' && aux[i] >= '0') || aux[i] == '.' || aux[i] == '-' )
        r += aux[i];
    }
    for(i = 107; i < aux.length()-2; i ++){
    if((aux[i] <= '9' && aux[i] >= '0') || aux[i] == '.' || aux[i] == '-' )
        p += aux[i];
    }
	
    dadosImu.yaw = std::stod(y.c_str());
    dadosImu.roll = std::stod(r.c_str());
    dadosImu.pitch = std::stod(p.c_str());
	
	roll = ceil(dadosImu.roll*pow(10,2)) / pow(10,2) ;
	pitch = ceil(dadosImu.pitch*pow(10,2)) / pow(10,2);
	yaw = ceil(dadosImu.yaw*pow(10,2)) / pow(10,2);
			
	//std::cout << "Yaw: " << yaw << std::endl;
	//std::cout << "Roll: " << roll << std::endl;
	//std::cout << "Pitch: " << pitch << std::endl;
    time3 = ((double)( clock() - ck3 )) / CLOCKS_PER_SEC;
    //cout << "Astra ---> Imu" << time3 << " seconds" << endl;
	ck4 = clock();
	imu_on = true;

	if(syncCond == 1){
		pthread_mutex_unlock(&mutexI);
		pthread_cond_signal(&cond3);
	}
	if(getStatus() == 1)
   		this->imu_sub.shutdown();
}

int SyncronizeDevices::getStatus()
{
	return status;
}

void SyncronizeDevices::setStatus(int val)
{
	status = val;
}


// Salvar Arquivos

void SyncronizeDevices::SaveFile(){
    int k;
    k = pthread_create(&fileThread,NULL,fileThreadFunc,(void*)this);
    pthread_join(fileThread,NULL);
    if(k)
		std::cout << std::endl <<"Falha laser" << std::endl; 
}

void* SyncronizeDevices::fileThreadFunc(void* arg)
{
    SyncronizeDevices* dev;
    dev = (SyncronizeDevices*)arg;
	pcl::io::savePCDFileASCII("laser_cloud.pcd", *dev->laserCloud);
	pcl::io::savePCDFileASCII("astra_cloud.pcd", *dev->astraCloud);
	
	std::ofstream arq;
	arq.open("imu_data.txt");				                			 
	arq << "Roll: " << dev->dadosImu.roll << std::endl;
	arq << "Pitch: " << dev->dadosImu.pitch << std::endl;
	arq << "Yaw: " << dev->dadosImu.yaw << std::endl;
    arq.close();
    std::cout << std::endl << std::endl << "Arquivos Salvos" << std::endl << std::endl;
}
