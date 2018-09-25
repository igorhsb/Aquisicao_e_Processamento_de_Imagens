#include "../include/project_pkg/Laser.h"

bool laser_on = false;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexZ = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexL = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexI = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond3 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond4 = PTHREAD_COND_INITIALIZER;


LaserS::LaserS(ros::NodeHandle n) :
    n_(n),
    status(0),
    laser_sub_(n_, "/base_scan", 10),
    laser_notifier_(laser_sub_,listener_, "laser", 10),
    laserCloud(new pcl::PointCloud<pcl::PointXYZRGB>)
{
	int s = 729*6;
	laserCloud->width = 729;
	laserCloud->height = 1;
	laserCloud->points.resize(729);
	laserPoints = (float*) malloc(s*sizeof(float));
    scan_sub = n_.subscribe("/scan", 1, &LaserS::scanCallback, this);
    
    //setStatus(1);
    //  scan_pub_ = n_.advertise<sensor_msgs::PointCloud>("/my_cloud",1);
    //scan_sub.shutdown();
}

LaserS::~LaserS(){
	scan_sub.shutdown();
	//setStatus(0);
	free(laserPoints);
}

void LaserS::scanCallback (const sensor_msgs::LaserScan::ConstPtr& scan_in)
{
std::cout << std::endl << "Laser waiting!" << std::endl << std::endl;
pthread_mutex_lock(&mutexL);
    if(syncronize_devices == true)
    {        
        
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
    	/*laserData[index].distancia= scan_in->ranges[index];
        laserData[index].angulo=(-135+(0.35*index));
        laserData[index].x = cloud.points[index].x;
        laserData[index].y = cloud.points[index].y;
        laserData[index].z = cloud.points[index].z;
        */
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
    std::vector<int> indicesNAN;
	//removeNaNFromPointCloud(*laserCloud, *laserCloud, indicesNAN);
	laser_on = true;
    if(syncronize_devices == true)
    {
        pthread_cond_signal(&cond2); 
    }
   pthread_mutex_unlock(&mutexL);
   if(getStatus() == 1){
   		this->scan_sub.shutdown();
   		laser_on = false;	
   }
}

void LaserS::Stop(){
	
}

void LaserS::SaveFile(){
    int k;
    k = pthread_create(&fileThread,NULL,fileThreadFunc,(void*)this);
    pthread_join(fileThread,NULL);
    if(k)
	std::cout << std::endl <<"Falha laser" << std::endl; 
}

void* LaserS::fileThreadFunc(void* arg)
{
    LaserS* l;
    l = (LaserS*)arg;
/*    std::ofstream arq;
    int i;

    arq.open("laser_data.txt");				  

    for (i = 0 ; i < 729; i++) {            			 
        arq << l->laserData[i].angulo << "  ";
        arq << l->laserData[i].distancia << "  ";
        arq << l->laserData[i].x << "  ";
        arq << l->laserData[i].y << "  ";
        arq << l->laserData[i].z;			 
        arq << std::endl;
    }
    arq.close();*/
  
  pcl::io::savePCDFileASCII("laser_cloud.pcd", *l->laserCloud);
  
    std::cout << std::endl << std::endl << "Laser File Created!" << std::endl << std::endl;
}

int LaserS::getStatus()
{
	return status;
}

void LaserS::setStatus(int val)
{
	status = val;
}

